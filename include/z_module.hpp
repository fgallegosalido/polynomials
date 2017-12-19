#pragma once

#include "aux.hpp"

#include <iostream>  // std::istream, std::ostream
#include <type_traits>  // std::make_unsigned_t

namespace detail{

   template <auto UInt>
   class ZModule{

      public:

         // Typedef for the value_type. We want it to be unsigned
         typedef std::make_unsigned_t<decltype(UInt)> value_type;
         // Variable holding the cardinal of the ring
         static constexpr value_type N = static_cast<value_type>(UInt);

         // Returns the notation name for the ring
         static constexpr std::string name();

         // Constructor by value
         constexpr ZModule (const value_type& zm = 0);

         // Increment and decrement operators
         constexpr ZModule& operator++ ();
         constexpr ZModule& operator-- ();
         constexpr ZModule operator++ (int);
         constexpr ZModule operator-- (int);

         // Operator overloadings for modular arithmetic
         constexpr ZModule& operator+= (const ZModule& zm);
         constexpr ZModule& operator-= (const ZModule& zm);
         constexpr ZModule& operator*= (const ZModule& zm);

         // Operator overloadings for compatibility with PODs
         template<typename U>
         constexpr ZModule& operator+= (const U& other);
         template<typename U>
         constexpr ZModule& operator-= (const U& other);
         template<typename U>
         constexpr ZModule& operator*= (const U& other);

         // Conversion to the underlined value_type (must use static_cast<>())
         constexpr explicit operator value_type() const;
         // Conversion to another integer ring (must use static_cast<>())
         template<auto UInt2>
         constexpr explicit operator ZModule<UInt2>() const;

         // I/O overloadings for ZModule objects
         friend std::istream& operator>> (std::istream& is, ZModule& zm){
            is >> zm.n;
            zm.n %= N;
            return is;
         }

         friend std::ostream& operator<< (std::ostream& os, const ZModule& zm){
            os << zm.n;
            return os;
         }

      protected:

         value_type n;
   };

   // Unary + and - operators
   template<auto UInt>
   constexpr ZModule<UInt> operator+ (const ZModule<UInt>& rhs);
   template<auto UInt>
   constexpr ZModule<UInt> operator- (const ZModule<UInt>& rhs);

   // Binary +, - and * operators for same type
   template<auto UInt>
   constexpr ZModule<UInt> operator+ (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);
   template<auto UInt>
   constexpr ZModule<UInt> operator- (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);
   template<auto UInt>
   constexpr ZModule<UInt> operator* (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);

   // Binary +, - and * operators for different types
   template<auto UInt, typename U>
   constexpr ZModule<UInt> operator+ (const ZModule<UInt>& lhs, const U& rhs);
   template<auto UInt, typename U>
   constexpr ZModule<UInt> operator- (const ZModule<UInt>& lhs, const U& rhs);
   template<auto UInt, typename U>
   constexpr ZModule<UInt> operator* (const ZModule<UInt>& lhs, const U& rhs);

   template<auto UInt, typename U>
   constexpr ZModule<UInt> operator+ (const U& lhs, const ZModule<UInt>& rhs);
   template<auto UInt, typename U>
   constexpr ZModule<UInt> operator- (const U& lhs, const ZModule<UInt>& rhs);
   template<auto UInt, typename U>
   constexpr ZModule<UInt> operator* (const U& lhs, const ZModule<UInt>& rhs);

   // Operator overloadings for comparisons of the same type
   template<auto UInt>
   constexpr bool operator== (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);
   template<auto UInt>
   constexpr bool operator!= (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);
   template<auto UInt>
   constexpr bool operator< (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);
   template<auto UInt>
   constexpr bool operator<= (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);
   template<auto UInt>
   constexpr bool operator> (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);
   template<auto UInt>
   constexpr bool operator>= (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);

   // Operator overloadings for comparisons with other types
   template<auto UInt, typename U>
   constexpr bool operator== (const ZModule<UInt>& lhs, const U& rhs);
   template<auto UInt, typename U>
   constexpr bool operator!= (const ZModule<UInt>& lhs, const U& rhs);
   template<auto UInt, typename U>
   constexpr bool operator< (const ZModule<UInt>& lhs, const U& rhs);
   template<auto UInt, typename U>
   constexpr bool operator<= (const ZModule<UInt>& lhs, const U& rhs);
   template<auto UInt, typename U>
   constexpr bool operator> (const ZModule<UInt>& lhs, const U& rhs);
   template<auto UInt, typename U>
   constexpr bool operator>= (const ZModule<UInt>& lhs, const U& rhs);

   template<auto UInt, typename U>
   constexpr bool operator== (const U& lhs, const ZModule<UInt>& rhs);
   template<auto UInt, typename U>
   constexpr bool operator!= (const U& lhs, const ZModule<UInt>& rhs);
   template<auto UInt, typename U>
   constexpr bool operator< (const U& lhs, const ZModule<UInt>& rhs);
   template<auto UInt, typename U>
   constexpr bool operator<= (const U& lhs, const ZModule<UInt>& rhs);
   template<auto UInt, typename U>
   constexpr bool operator> (const U& lhs, const ZModule<UInt>& rhs);
   template<auto UInt, typename U>
   constexpr bool operator>= (const U& lhs, const ZModule<UInt>& rhs);


   // Implementations of all the functions
   #include "../source/z_module.cpp"
   #include "../source/z_module_arithmetics.cpp"
   #include "../source/z_module_comparisons.cpp"
}
