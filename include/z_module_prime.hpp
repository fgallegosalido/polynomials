#pragma once

#include "aux.hpp"
#include "z_module.hpp"

#include <type_traits>  // std::make_unsigned_t

// This macro will let oveloadings of some operators for ZModulePrime
#define INCLUDED_Z_MODULE_PRIME


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
   #include "../source/z_module_comparisons.cpp"
}
