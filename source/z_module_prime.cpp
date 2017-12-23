/************* Constructor *************/
template<auto Integral>
constexpr ZModulePrime<Integral>::ZModulePrime (const value_type& zm) : ZModule<Integral>(zm){}

/*********************************************************************/
/************* Operator /= for same and different types **************/
/*********************************************************************/
template<auto Integral>
constexpr ZModulePrime<Integral>& ZModulePrime<Integral>::operator/= (const ZModulePrime<Integral>& zm){
   n = (n*zm.inverse())%N;
   return *this;
}
template<auto Integral> template<typename U>
constexpr ZModulePrime<Integral>& ZModulePrime<Integral>::operator/= (const U& other){
   n = (n * ZModulePrime<Integral>(static_cast<value_type>(other)).inverse())%N;
   return *this;
}

/********************************************************************/
/************* Operator / for same and different types **************/
/********************************************************************/
template<auto Integral>
constexpr ZModulePrime<Integral> operator/ (const ZModulePrime<Integral>& lhs, const ZModulePrime<Integral>& rhs){
   return ZModulePrime<Integral>(lhs) /= rhs;
}

template<auto Integral, typename U>
constexpr ZModulePrime<Integral> operator/ (const ZModulePrime<Integral>& lhs, const U& rhs){
   return ZModulePrime<Integral>(lhs) /= static_cast<typename ZModulePrime<Integral>::value_type>(rhs);
}
template<auto Integral, typename U>
constexpr ZModulePrime<Integral> operator/ (const U& lhs, const ZModulePrime<Integral>& rhs){
   return ZModulePrime<Integral>(static_cast<typename ZModulePrime<Integral>::value_type>(lhs)) /= rhs;
}

// Calculate the inverse of the number in the ring
template<auto Integral>
constexpr auto ZModulePrime<Integral>::inverse() const{
   if (n == 0){
      throw std::domain_error("Divide by zero exception");
   }
   // TODO: Implement Extended Euclidean Algorithm, this one is slow
   // TODO: Use a constexpr pattern to force compile-time evaluation
   value_type ret=1;
   while ((n*ret)%N != 1) ++ret;
   return ret;
}

// Conversion to another prime integer ring (must use static_cast<>())
template<auto Integral> template<auto Integral2>
constexpr ZModulePrime<Integral>::operator ZModulePrime<Integral2>() const{
   return ZModulePrime<Integral2>(n);
}
