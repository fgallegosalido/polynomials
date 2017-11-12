#pragma once

#include <type_traits>
#include <cmath>

namespace detail{

   #ifdef PRIME_CHECK_SUPPORT
   namespace aux{
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
            static const bool result = prime_helper<static_cast<value_type>(N),
                                       static_cast<value_type>(std::sqrt(N)+1)>::result;
         };

      }  // namespace primes
   }  // namespace aux
   #endif

   template <auto I>
   class ZModule{

      public:

         typedef std::make_unsigned_t<decltype(I)> value_type;
         static constexpr value_type N = static_cast<value_type>(I);

         constexpr ZModule (const value_type& zm = 0) : n(zm%N){}

         // Swap two numbers
         friend void swap (ZModule& lhs, ZModule& rhs){
            using std::swap;
            swap(lhs.n, rhs.n);
         }

         // Operator overloadings for modular arithmetic
         ZModule& operator+= (const ZModule& zm){
            n = (n+zm.n)%N;
            return *this;
         }

         ZModule& operator-= (const ZModule& zm){
            n = (n+N-zm.n)%N;
            return *this;
         }

         ZModule& operator*= (const ZModule& zm){
            n = (n*zm.n)%N;
            return *this;
         }

         inline const ZModule operator+ (const ZModule& zm) const{
            return ZModule(*this) += zm;
         }

         inline const ZModule operator- (const ZModule& zm) const{
            return ZModule(*this) -= zm;
         }

         inline const ZModule operator* (const ZModule& zm) const{
            return ZModule(*this) *= zm;
         }

      protected:

         value_type n;
   };

   template <auto I>
   class ZModulePrime : public ZModule<I>{

      public:

         typedef typename ZModule<I>::value_type value_type;
         static constexpr value_type N = ZModule<I>::N;

         ZModulePrime& operator/= (const ZModulePrime& zm){
            this->n = ((this->n)*inverse(zm))%N;
            return *this;
         }

         inline const ZModulePrime operator/ (const ZModulePrime& zm) const{
            return ZModulePrime(*this) /= zm;
         }

      private:

         const value_type inverse(const value_type& a){
            value_type ret=1;
            while ((a*ret)%N != 1) ++ret;
            return ret;
         }
   };
}
