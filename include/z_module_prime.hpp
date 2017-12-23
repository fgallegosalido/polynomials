#pragma once

#include "aux.hpp"
#include "z_module.hpp"

#include <exception> // std::domain_error

// This macro will let oveloadings of some operators for ZModulePrime
#define INCLUDED_Z_MODULE_PRIME


namespace detail{

   template <auto Integral>
   class ZModulePrime : public ZModule<Integral>{
      // In general, this class should have a prime number cardinality,
      // so we will throw a compile error if the user tries to instantiate
      // a non-prime cardinality (if PRIME_CHECK_SUPPORT is enabled)
      static_assert(aux::is_prime(Integral),
            "The cardinal of a ZModulePrime must be prime.");

      public:

         // Take the value_type of the base class, ZModule<Integral>
         typedef typename ZModule<Integral>::value_type value_type;
         using ZModule<Integral>::N;
         using ZModule<Integral>::n;

         // Constructor by value
         constexpr ZModulePrime (const value_type& zm = 0);

         // Operator /= overloadings for division (multiply by the inverse)
         constexpr ZModulePrime& operator/= (const ZModulePrime& zm);
         template<typename U>
         constexpr ZModulePrime& operator/= (const U& other);

         // Conversion to another integer ring (must use static_cast<>())
         template<auto Integral2>
         constexpr explicit operator ZModulePrime<Integral2>() const;

      private:
         // Helper function to calculate the inverse of the number in this ring
         constexpr auto inverse() const;
   };

   // Binary / operator for same type
   template<auto Integral>
   constexpr ZModulePrime<Integral> operator/ (const ZModulePrime<Integral>& lhs, const ZModulePrime<Integral>& rhs);

   // Binary / operator for different types
   template<auto Integral, typename U>
   constexpr ZModulePrime<Integral> operator/ (const ZModulePrime<Integral>& lhs, const U& rhs);
   template<auto Integral, typename U>
   constexpr ZModulePrime<Integral> operator/ (const U& lhs, const ZModulePrime<Integral>& rhs);


   // Implementations of all the functions
   #include "../source/z_module_prime.cpp"
   #include "../source/z_module_arithmetics.cpp"
   #include "../source/z_module_comparisons.cpp"
}
