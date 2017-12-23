#pragma once

#include "aux.hpp"

#include <iostream>  // std::istream, std::ostream
#include <string> // std::string

namespace detail{

   template <auto Integral>
   class ZModule{
      /* It makes no sense to have a cardinality lower than 2,
       * so we will force it to be bigger than 1.
       */
      static_assert(Integral > 1, "Cardinal of Z-module must be bigger than 1");

      public:

         // Typedef for the value_type
         typedef decltype(Integral) value_type;
         // Variable holding the cardinal of the ring
         static constexpr value_type N = Integral;

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
         template<typename T = value_type>
         constexpr explicit operator T() const;
         // Conversion to another integer ring (must use static_cast<>())
         template<auto Integral2>
         constexpr explicit operator ZModule<Integral2>() const;

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
   template<auto Integral>
   constexpr ZModule<Integral> operator+ (const ZModule<Integral>& rhs);
   template<auto Integral>
   constexpr ZModule<Integral> operator- (const ZModule<Integral>& rhs);

   // Binary +, - and * operators for same type
   template<auto Integral>
   constexpr ZModule<Integral> operator+ (const ZModule<Integral>& lhs, const ZModule<Integral>& rhs);
   template<auto Integral>
   constexpr ZModule<Integral> operator- (const ZModule<Integral>& lhs, const ZModule<Integral>& rhs);
   template<auto Integral>
   constexpr ZModule<Integral> operator* (const ZModule<Integral>& lhs, const ZModule<Integral>& rhs);

   // Binary +, - and * operators for different types
   template<auto Integral, typename U>
   constexpr ZModule<Integral> operator+ (const ZModule<Integral>& lhs, const U& rhs);
   template<auto Integral, typename U>
   constexpr ZModule<Integral> operator- (const ZModule<Integral>& lhs, const U& rhs);
   template<auto Integral, typename U>
   constexpr ZModule<Integral> operator* (const ZModule<Integral>& lhs, const U& rhs);

   template<auto Integral, typename U>
   constexpr ZModule<Integral> operator+ (const U& lhs, const ZModule<Integral>& rhs);
   template<auto Integral, typename U>
   constexpr ZModule<Integral> operator- (const U& lhs, const ZModule<Integral>& rhs);
   template<auto Integral, typename U>
   constexpr ZModule<Integral> operator* (const U& lhs, const ZModule<Integral>& rhs);

   // Operator overloadings for comparisons of the same type
   template<auto Integral>
   constexpr bool operator== (const ZModule<Integral>& lhs, const ZModule<Integral>& rhs);
   template<auto Integral>
   constexpr bool operator!= (const ZModule<Integral>& lhs, const ZModule<Integral>& rhs);
   template<auto Integral>
   constexpr bool operator< (const ZModule<Integral>& lhs, const ZModule<Integral>& rhs);
   template<auto Integral>
   constexpr bool operator<= (const ZModule<Integral>& lhs, const ZModule<Integral>& rhs);
   template<auto Integral>
   constexpr bool operator> (const ZModule<Integral>& lhs, const ZModule<Integral>& rhs);
   template<auto Integral>
   constexpr bool operator>= (const ZModule<Integral>& lhs, const ZModule<Integral>& rhs);

   // Operator overloadings for comparisons with other types
   template<auto Integral, typename U>
   constexpr bool operator== (const ZModule<Integral>& lhs, const U& rhs);
   template<auto Integral, typename U>
   constexpr bool operator!= (const ZModule<Integral>& lhs, const U& rhs);
   template<auto Integral, typename U>
   constexpr bool operator< (const ZModule<Integral>& lhs, const U& rhs);
   template<auto Integral, typename U>
   constexpr bool operator<= (const ZModule<Integral>& lhs, const U& rhs);
   template<auto Integral, typename U>
   constexpr bool operator> (const ZModule<Integral>& lhs, const U& rhs);
   template<auto Integral, typename U>
   constexpr bool operator>= (const ZModule<Integral>& lhs, const U& rhs);

   template<auto Integral, typename U>
   constexpr bool operator== (const U& lhs, const ZModule<Integral>& rhs);
   template<auto Integral, typename U>
   constexpr bool operator!= (const U& lhs, const ZModule<Integral>& rhs);
   template<auto Integral, typename U>
   constexpr bool operator< (const U& lhs, const ZModule<Integral>& rhs);
   template<auto Integral, typename U>
   constexpr bool operator<= (const U& lhs, const ZModule<Integral>& rhs);
   template<auto Integral, typename U>
   constexpr bool operator> (const U& lhs, const ZModule<Integral>& rhs);
   template<auto Integral, typename U>
   constexpr bool operator>= (const U& lhs, const ZModule<Integral>& rhs);


   // Implementations of all the functions
   #include "../source/z_module.cpp"
   #include "../source/z_module_arithmetics.cpp"
   #include "../source/z_module_comparisons.cpp"
}
