/************* Constructor *************/
template<auto UInt>
constexpr ZModulePrime<UInt>::ZModulePrime (const value_type& zm) : ZModule<UInt>(zm){}

/*********************************************************************/
/************* Operator /= for same and different types **************/
/*********************************************************************/
template<auto UInt>
constexpr ZModulePrime<UInt>& ZModulePrime<UInt>::operator/= (const ZModulePrime<UInt>& zm){
   n = (n*zm.inverse())%N;
   return *this;
}
template<auto UInt> template<typename U>
constexpr ZModulePrime<UInt>& ZModulePrime<UInt>::operator/= (const U& other){
   n = (n * ZModulePrime<UInt>(static_cast<value_type>(other)).inverse())%N;
   return *this;
}

/********************************************************************/
/************* Operator / for same and different types **************/
/********************************************************************/
template<auto UInt>
constexpr ZModulePrime<UInt> operator/ (const ZModulePrime<UInt>& lhs, const ZModulePrime<UInt>& rhs){
   return ZModulePrime<UInt>(lhs) /= rhs;
}

template<auto UInt, typename U>
constexpr ZModulePrime<UInt> operator/ (const ZModulePrime<UInt>& lhs, const U& rhs){
   return ZModulePrime<UInt>(lhs) /= static_cast<typename ZModulePrime<UInt>::value_type>(rhs);
}
template<auto UInt, typename U>
constexpr ZModulePrime<UInt> operator/ (const U& lhs, const ZModulePrime<UInt>& rhs){
   return ZModulePrime<UInt>(static_cast<typename ZModulePrime<UInt>::value_type>(lhs)) /= rhs;
}

// Calculate the inverse of the number in the ring
template<auto UInt>
constexpr auto ZModulePrime<UInt>::inverse() const{
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
template<auto UInt> template<auto UInt2>
constexpr ZModulePrime<UInt>::operator ZModulePrime<UInt2>() const{
   return ZModulePrime<UInt2>(n);
}
