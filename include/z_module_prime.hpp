#pragma once

#include "z_module.hpp"

#include <limits>
#include <type_traits>

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

   #else
   template<typename U>
   constexpr bool is_prime (U){
      return true;
   }
   #endif // PRIME_CHECK_SUPPORT

}  // namespace detail::aux

namespace detail{

   template <auto UInt, typename = std::enable_if_t<aux::is_prime(
                  static_cast<std::make_unsigned_t<decltype(UInt)>>(UInt))>>
   class ZModulePrime : public ZModule<UInt>{

      public:

         typedef typename ZModule<UInt>::value_type value_type;
         static constexpr value_type N = static_cast<value_type>(UInt);

         ZModulePrime (const value_type& zm = 0);

         ZModulePrime& operator/= (const ZModulePrime& zm);
         template<typename U>
         ZModulePrime& operator/= (const U& other);

         auto inverse() const;

         // Conversion to another integer ring (must use static_cast<>())
         template<auto UInt2>
         explicit operator ZModulePrime<UInt2>() const;
   };

   template<auto UInt>
   ZModulePrime<UInt> operator/ (const ZModulePrime<UInt>& lhs, const ZModulePrime<UInt>& rhs);

   template<auto UInt, typename U>
   ZModulePrime<UInt> operator/ (const ZModulePrime<UInt>& lhs, const U& rhs);
   template<auto UInt, typename U>
   ZModulePrime<UInt> operator/ (const U& lhs, const ZModulePrime<UInt>& rhs);


   #include "../source/z_module_prime.cpp"
}
