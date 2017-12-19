#pragma once

#include "z_module.hpp"

#include <limits> // std::numeric_limits<U>::digits
#include <type_traits>  // std::make_unsigned_t

// This macro will let oveloadings of some operators for ZModulePrime
#define INCLUDED_Z_MODULE_PRIME

namespace detail::aux{
   /* Primality test for compile-time evaluation
    *
    * Source code taken from Casey's answer at Stack Overflow:
    * https://stackoverflow.com/questions/18303632/compile-time-prime-checking
    */
   #ifdef PRIME_CHECK_SUPPORT // Activation via macro
   namespace primes{

      template<typename U>
      constexpr U mid(U low, U high) {
         return (low + high) / 2;
      }

      // precondition: low*low <= n, high*high > n.
      template<typename U>
      constexpr U ceilsqrt (U n, U low, U high){
         return low + 1 >= high
            ? high
            : (mid(low, high) * mid(low, high) == n)
               ? mid(low, high)
               : (mid(low, high) * mid(low, high) <  n)
                  ? ceilsqrt(n, mid(low, high), high)
                  : ceilsqrt(n, low, mid(low, high));
      }

      // returns ceiling(sqrt(n))
      template<typename U>
      constexpr U ceilsqrt (U n){
         return n < 3
            ? n
               : ceilsqrt(n, U(1), U(1) << (std::numeric_limits<U>::digits / 2));
      }


      // returns true if n is divisible by an odd integer in
      // [2 * low + 1, 2 * high + 1).
      template<typename U>
      constexpr bool find_factor (U n, U low, U high){
         return low + 1 >= high
            ? (n % (2 * low + 1)) == 0
               : (find_factor(n, low, mid(low, high))
                  || find_factor(n, mid(low, high), high));
      }

   }  // namespace primes

   template<typename U>
   constexpr bool is_prime (U n){
      return n > 1
         && (n == 2
            || (n % 2 == 1
               && (n == 3
                  || !primes::find_factor(n, U(1), (primes::ceilsqrt(n) + 1) / 2))));
   }

   #else // If the user decides not to check for primality, is_prime() will return true
   template<typename U>
   constexpr bool is_prime (U){
      return true;
   }
   #endif // PRIME_CHECK_SUPPORT

}  // namespace detail::aux

namespace detail{

   template <auto UInt>
   class ZModulePrime : public ZModule<UInt>{
      // In general, this class should have a prime number cardinality,
      // so we will throw a compile error if the user tries to instantiate
      // a non-prime prime cardinality (if PRIME_CHECK_SUPPORT is enabled)
      static_assert(aux::is_prime(static_cast<std::make_unsigned_t<decltype(UInt)>>(UInt)),
            "The cardinal of a ZModulePrime must be prime.");

      public:

         // Take the value_type of the base class, ZModule
         typedef typename ZModule<UInt>::value_type value_type;
         static constexpr value_type N = static_cast<value_type>(UInt);

         // Constructor by value
         ZModulePrime (const value_type& zm = 0);

         // Operator /= overloadings for division (multiply by the inverse)
         ZModulePrime& operator/= (const ZModulePrime& zm);
         template<typename U>
         ZModulePrime& operator/= (const U& other);

         // Conversion to another integer ring (must use static_cast<>())
         template<auto UInt2>
         explicit operator ZModulePrime<UInt2>() const;

      private:
         // Helper function to calculate the inverse of the number in this ring
         auto inverse() const;
   };

   // Binary / operator for same type
   template<auto UInt>
   ZModulePrime<UInt> operator/ (const ZModulePrime<UInt>& lhs, const ZModulePrime<UInt>& rhs);

   // Binary / operator for different types
   template<auto UInt, typename U>
   ZModulePrime<UInt> operator/ (const ZModulePrime<UInt>& lhs, const U& rhs);
   template<auto UInt, typename U>
   ZModulePrime<UInt> operator/ (const U& lhs, const ZModulePrime<UInt>& rhs);


   // Implementations of all the functions
   #include "../source/z_module_prime.cpp"
   #include "../source/z_module_arithmetics.cpp"
}
