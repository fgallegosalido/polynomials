#pragma once

#include <iostream>
#include <type_traits>
#include <cmath>

namespace detail{

   template <auto UInt>
   class ZModule{

      public:

         typedef std::make_unsigned_t<decltype(UInt)> value_type;
         static constexpr value_type N = static_cast<value_type>(UInt);

         ZModule (const value_type& zm = 0);

         // Increment and decrement operators
         ZModule& operator++ ();
         ZModule& operator-- ();
         ZModule operator++ (int);
         ZModule operator-- (int);

         // Unary + and - operators
         ZModule operator+ () const;
         ZModule operator- () const;

         // Operator overloadings for modular arithmetic
         ZModule& operator+= (const ZModule& zm);
         ZModule& operator-= (const ZModule& zm);
         ZModule& operator*= (const ZModule& zm);

         // Operator overloadings for compatibility with PODs
         template<typename U>
         ZModule& operator+= (const U& other);
         template<typename U>
         ZModule& operator-= (const U& other);
         template<typename U>
         ZModule& operator*= (const U& other);

         // Operator overloadings for comparisons of the same type
         bool operator== (const ZModule& zm) const;
         bool operator!= (const ZModule& zm) const;
         bool operator< (const ZModule& zm) const;
         bool operator<= (const ZModule& zm) const;
         bool operator> (const ZModule& zm) const;
         bool operator>= (const ZModule& zm) const;

         // Operator overloadings for comparisons with other types
         template<typename U>
         bool operator== (const U& zm) const;
         template<typename U>
         bool operator!= (const U& zm) const;
         template<typename U>
         bool operator< (const U& zm) const;
         template<typename U>
         bool operator<= (const U& zm) const;
         template<typename U>
         bool operator> (const U& zm) const;
         template<typename U>
         bool operator>= (const U& zm) const;

         // Conversion to another integer ring (must use static_cast<>())
         template<auto UInt2>
         explicit operator ZModule<UInt2>() const;
         // Conversion to the underlined value_type
         operator value_type() const;

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

   // Binary +, - and * operators for same type
   template<auto UInt>
   ZModule<UInt> operator+ (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);
   template<auto UInt>
   ZModule<UInt> operator- (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);
   template<auto UInt>
   ZModule<UInt> operator* (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);

   // Binary +, - and * operators for different types
   template<auto UInt, typename U>
   ZModule<UInt> operator+ (const ZModule<UInt>& lhs, const U& rhs);
   template<auto UInt, typename U>
   ZModule<UInt> operator- (const ZModule<UInt>& lhs, const U& rhs);
   template<auto UInt, typename U>
   ZModule<UInt> operator* (const ZModule<UInt>& lhs, const U& rhs);

   template<auto UInt, typename U>
   ZModule<UInt> operator+ (const U& lhs, const ZModule<UInt>& rhs);
   template<auto UInt, typename U>
   ZModule<UInt> operator- (const U& lhs, const ZModule<UInt>& rhs);
   template<auto UInt, typename U>
   ZModule<UInt> operator* (const U& lhs, const ZModule<UInt>& rhs);


   #include "../source/z_module.cpp"   // Implementation
}
