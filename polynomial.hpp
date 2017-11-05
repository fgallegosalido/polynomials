#pragma once

#include <iostream>  // std::cout, std::cin, std::endl
#include <vector> // std::vector
#include <string> // std::string
#include <initializer_list>   // std::initializer_list
#include <iterator>  // std::iterator_traits
#include <cmath>  // std::pow()
#include <complex>   // std::complex
#include <boost/rational.hpp> // boost::rational

namespace aux{

   /* Array that contains numbers from 0 to 9 (including empty string)
    *
    * If the user defines the macro UNICODE_SUPPORT, an array of superscript
    * numbers will be generated instead of normal ones
    */
   constexpr const char* superscripts[] = {
#ifdef UNICODE_SUPPORT
   "\u2070", "\u00B9", "\u00B2", "\u00B3", "\u2074", "\u2075", "\u2076", "\u2077", "\u2078", "\u2079"
#else
   "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
#endif
   , ""};

   /* Function that returns a number from 0 to 9 in superscript (if enabled
    * by the user)
    */
   auto superscript (size_t n){
      if (n<=9) return superscripts[n];
      return superscripts[10];   // Any other number will return an empty string
   }

   /* Function that returns any unsigned number in superscript.
    *
    * If UNICODE_SUPPORT is not enabled, it will return the string "^n"
    */
   std::string power_string (unsigned  n){
      std::string power = "";

#ifdef UNICODE_SUPPORT
      if (n==1 || n==0) return power;

      while (n>0){
         power = superscript(n%10) + power;
         n /= 10;
      }
#else
      power = "^" + std::to_string(n);
#endif

      return power;
   }

}  // namespace aux

// Alias to get the category of an iterator (random access, bidirectional,...)
template <typename Iterator>
using traits = typename std::iterator_traits<Iterator>::iterator_category;

// Class Polynomial. CType is the type of the coefficients
template <typename CType>
class Polynomial{

   public:

      // typedefs for the value_type (coefficients) and the size_type
      typedef typename std::vector<CType>::value_type value_type;
      typedef typename std::vector<CType>::size_type size_type;

      explicit Polynomial (char c = 'x') : var(c), coeffs(1, 0){}
      Polynomial (const Polynomial& other) noexcept : var(other.var), coeffs(other.coeffs){}
      Polynomial (Polynomial&& other) noexcept : var(std::move(other.var)), coeffs(std::move(other.coeffs)){}
      Polynomial (std::initializer_list<value_type> l, char c = 'x') : var(c), coeffs(l) {adjust_grade();}

      template <typename InputIterator>
      Polynomial (InputIterator first, InputIterator last, char c = 'x')
      : Polynomial(first, last, c, traits<InputIterator>()) {}

      Polynomial& operator= (const Polynomial& pol){
         if (this != &pol){
            var = pol.var;
            coeffs = pol.coeffs;
         }
         return *this;
      }

      value_type get_coefficient (size_type i) const{
         if (i<coeffs.size()){
            return coeffs[i];
         }
         else{
            return value_type(0);
         }
      }

      void set_coefficient (size_type i, value_type elem){
         if (i>=coeffs.size()){
            coeffs.resize(i+1, 0);
         }

         coeffs[i] = elem;
         adjust_grade();
      }

      inline size_type grade () const{
         return coeffs.size()-1;
      }

      inline char variable () const{
         return var;
      }

      template<typename RType>
      RType evaluate_at (RType x) const{
         RType res = static_cast<RType>(coeffs[0]);
         for (size_type i=1; i<coeffs.size(); ++i){
            res += static_cast<RType>(coeffs[i])*std::pow(x, i);
         }

         return res;
      }

      friend void swap (Polynomial& lhs, Polynomial& rhs){
         using std::swap;

         swap(lhs.coeffs, rhs.coeffs);
         swap(lhs.var, rhs.var);
      }

      Polynomial& operator+= (const Polynomial& pol){
         size_type size = pol.coeffs.size();
         if (size > coeffs.size()) coeffs.resize(size, 0);

         for (size_type i=0; i<size; ++i){
            coeffs[i] += pol.coeffs[i];
         }

         if (size == coeffs.size()) adjust_grade();

         return *this;
      }

      Polynomial& operator-= (const Polynomial& pol){
         size_type size = pol.coeffs.size();
         if (size > coeffs.size()) coeffs.resize(size, 0);

         for (size_type i=0; i<size; ++i){
            coeffs[i] -= pol.coeffs[i];
         }

         if (size == coeffs.size()) adjust_grade();

         return *this;
      }

      Polynomial& operator*= (const Polynomial& pol){
         coeffs.resize(coeffs.size()+pol.coeffs.size()-1, 0);

         for (int i=coeffs.size()-pol.coeffs.size(); i>=0; --i){
            for (size_type j=pol.coeffs.size()-1; j>0; --j){
               coeffs[i+j] += coeffs[i]*pol.coeffs[j];
            }
            coeffs[i] = coeffs[i]*pol.coeffs[0];
         }

         return *this;
      }

      Polynomial& operator/= (const Polynomial& pol){
         if (coeffs.size() < pol.coeffs.size()){
            coeffs.resize(1);
            coeffs[0] = 0;
            return *this;
         }

         std::vector<value_type> coc(coeffs.size()-pol.coeffs.size()+1, 0);

         for (int i=coc.size()-1; i>=0; --i){
            coc[i] = coeffs[pol.coeffs.size()+i-1]/pol.coeffs.back();
            for (int j=pol.coeffs.size()-2; j>=0; --j){
               coeffs[i+j] -= pol.coeffs[j]*coc[i];
            }
         }

         coeffs = std::move(coc);
         return *this;
      }

      Polynomial& operator%= (const Polynomial& pol){
         if (coeffs.size() < pol.coeffs.size()) return *this;

         value_type coc;

         for (int i=coeffs.size()-pol.coeffs.size(); i>=0; --i){
            coc = coeffs[pol.coeffs.size()+i-1]/pol.coeffs.back();
            for (int j=pol.coeffs.size()-1; j>=0; --j){
               coeffs[i+j] -= pol.coeffs[j]*coc;
            }
         }

         adjust_grade();
         return *this;
      }

      inline const Polynomial operator+ (const Polynomial& pol)const{
         return Polynomial(*this) += pol;
      }

      inline const Polynomial operator- (const Polynomial& pol)const{
         return Polynomial(*this) -= pol;
      }

      inline const Polynomial operator* (const Polynomial& pol)const{
         return Polynomial(*this) *= pol;
      }

      inline const Polynomial operator/ (const Polynomial& pol)const{
         return Polynomial(*this) /= pol;
      }

      inline const Polynomial operator% (const Polynomial& pol)const{
         return Polynomial(*this) %= pol;
      }

      bool operator== (const Polynomial& pol) const{
         if (coeffs.size() != pol.coeffs.size()) return false;

         for (size_type i=0; i<coeffs.size(); ++i){
            if (coeffs[i] != pol.coeffs[i]) return false;
         }

         return true;
      }

      bool operator!= (const Polynomial& pol) const{
         return !(*this == pol);
      }

      Polynomial& differentiate (){
         if (coeffs.size() == 1){
            coeffs[0] = 0;
         }
         else{
            for (size_type i=1; i<coeffs.size(); ++i){
               coeffs[i-1] = coeffs[i]*static_cast<value_type>(i);
            }
            coeffs.resize(coeffs.size()-1);
         }

         return *this;
      }

      Polynomial& integrate_const (const value_type& c = 0){
         if (coeffs.size()==1 && coeffs[0]==0){
            coeffs[0] = c;
            return *this;
         }

         coeffs.resize(coeffs.size()+1);

         for (size_type i=coeffs.size()-1; i>0; --i){
            coeffs[i] = coeffs[i-1]/static_cast<value_type>(i);
         }
         coeffs[0] = c;

         return *this;
      }

      template<typename RType>
      Polynomial& integrate_point (const RType& x, const RType& y){
         coeffs[0] = static_cast<value_type>(y - (*this).integrate_const().evaluate_at(x));
         return *this;
      }

      template <typename RType>
      RType definite_integral (const RType& lower_bound, const RType& upper_bound){
         Polynomial p(*this);
         p.integrate_const();
         return p.evaluate_at(upper_bound) - p.evaluate_at(lower_bound);
      }

      // friend std::istream& operator>>(std::istream& is, Polynomial& pol);
      friend std::ostream& operator<< (std::ostream& os, const Polynomial& pol){
         os.precision(5);

         if (pol.grade() == 0){
            os << pol.get_coefficient(0);
         }
         else{
            if (pol.get_coefficient(pol.grade()) == -1){
               os << "-";
            }
            else if (pol.get_coefficient(pol.grade()) != 1){
               os << pol.get_coefficient(pol.grade());
            }
            os << pol.variable() << aux::power_string(pol.grade());

            for (size_type i=pol.grade()-1; i>0; --i){
               if (pol.get_coefficient(i) != 0){
                  if (pol.get_coefficient(i) > 0){
                     os << "+";

                     if (pol.get_coefficient(i) != 1){
                        os << pol.get_coefficient(i);
                     }
                  }
                  else if (pol.get_coefficient(i) == -1){
                     os << "-";
                  }

                  os << pol.variable() << aux::power_string(i);
               }
            }

            if (pol.get_coefficient(0) != 0){
               if (pol.get_coefficient(0) > 0){
                  os << "+";
               }
               os << pol.get_coefficient(0);
            }
         }

         return os;
      }

   private:

      char var;
      std::vector<value_type> coeffs;

      void adjust_grade (){
         while (coeffs.back()==0 && coeffs.size()>1){
            coeffs.pop_back();
         }
      }

      template <typename InputIterator>
      Polynomial (InputIterator first, InputIterator last, char c, std::input_iterator_tag)
      : var(c), coeffs(first, last) {adjust_grade();}

};

template <typename CType>
const Polynomial<CType> differentiate (const Polynomial<CType>& pol){
   return Polynomial<CType>(pol).differentiate();
}

template <typename CType>
const Polynomial<CType> integrate_const (const Polynomial<CType>& pol, const CType& c = 0){
   return Polynomial<CType>(pol).integrate_const(c);
}

template <typename CType>
const Polynomial<CType> gcd (const Polynomial<CType>& lhs, const Polynomial<CType>& rhs){
   auto p1(lhs);
   auto p2(rhs);

   if (lhs.grade() < rhs.grade()){
      swap(p1, p2);
   }

   while (p2.grade()>0 || p2.get_coefficient(0)!=0){
      swap(p1, p2);
      p2 %= p1;
   }

   return p1;
}

template <typename CType>
const Polynomial<CType> lcm (const Polynomial<CType>& lhs, const Polynomial<CType>& rhs){
   return (lhs*rhs)/gcd(lhs, rhs);
}

// Typedefs for coefficients with integer types
typedef Polynomial<int8_t> polynomial_int8;
typedef Polynomial<int16_t> polynomial_int16;
typedef Polynomial<int32_t> polynomial_int32;
typedef Polynomial<int64_t> polynomial_int64;

// Typedefs for coefficients in floating point (real numbers)
typedef Polynomial<float> polynomial_float32;
typedef Polynomial<double> polynomial_float64;

// Typedefs for extra types (boost::rational and std::complex)
typedef Polynomial<boost::rational<int8_t>> polynomial_rational_int8;
typedef Polynomial<boost::rational<int16_t>> polynomial_rational_int16;
typedef Polynomial<boost::rational<int32_t>> polynomial_rational_int32;
typedef Polynomial<boost::rational<int64_t>> polynomial_rational_int64;

typedef Polynomial<std::complex<float>> polynomial_complex_float32;
typedef Polynomial<std::complex<double>> polynomial_complex_float64;
