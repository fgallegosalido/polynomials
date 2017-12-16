#pragma once

#include <iostream>
#include <type_traits>
#include <cmath>

namespace detail{

   namespace aux{
      #ifdef PRIME_CHECK_SUPPORT
      namespace primes {
         template <auto N, auto D>
         struct prime_helper {
            static constexpr bool result = (N%D) && prime_helper<N, D-1>::result;
         };

         template <auto N>
         struct prime_helper<N,1> {
            static constexpr bool result = true;
         };

         template <auto N>
         struct is_prime {
            typedef std::make_unsigned_t<decltype(N)> value_type;
            static constexpr bool result = prime_helper<static_cast<value_type>(N),
                                       static_cast<value_type>(std::sqrt(N)+1)>::result;
         };

      }  // namespace primes
      #endif

      template<typename T>
      struct is_z_module{

      };
   }  // namespace aux

   template <auto UInt>
   class ZModule{

      public:

         typedef std::make_unsigned_t<decltype(UInt)> value_type;
         static constexpr value_type N = static_cast<value_type>(UInt);

         ZModule (const value_type& zm = 0);

         ZModule& operator++ ();
         ZModule& operator-- ();
         ZModule operator++ (int);
         ZModule operator-- (int);

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

         // Operator overloadings for comparisons of the same type
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

   template<auto UInt>
   ZModule<UInt> operator+ (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);
   template<auto UInt>
   ZModule<UInt> operator- (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);
   template<auto UInt>
   ZModule<UInt> operator* (const ZModule<UInt>& lhs, const ZModule<UInt>& rhs);


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

   template <auto UInt>
   class ZModulePrime : public ZModule<UInt>{

      public:

         typedef typename ZModule<UInt>::value_type value_type;
         static constexpr value_type N = static_cast<value_type>(UInt);

         ZModulePrime (const value_type& zm = 0) : ZModule<UInt>(zm){}

         ZModulePrime& operator/= (const ZModulePrime& zm){
            this->n = ((this->n)*inverse(zm))%N;
            return *this;
         }

         ZModulePrime operator/ (const ZModulePrime& zm) const{
            return ZModulePrime(*this) /= zm;
         }

         const value_type inverse(){
            // TODO: Use Extended Euclidean algorithm
            value_type ret=1;
            while (((this->n)*ret)%N != 1) ++ret;
            return ret;
         }

         // Conversion to another integer ring (must use static_cast<>())
         template<auto UInt2>
         operator ZModulePrime<UInt2>() const{
            return ZModulePrime<UInt2>(this->n);
         }
   };



   #include "../source/z_module.cpp"
}
