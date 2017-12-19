/************* Constructor *************/
template<auto UInt>
ZModulePrime<UInt>::ZModulePrime (const value_type& zm) : ZModule<UInt>(zm){}

/*********************************************************************/
/************* Operator /= for same and different types **************/
/*********************************************************************/
template<auto UInt>
ZModulePrime<UInt>& ZModulePrime<UInt>::operator/= (const ZModulePrime<UInt>& zm){
   this->n = ((this->n)*zm.inverse())%N;
   return *this;
}
template<auto UInt> template<typename U>
ZModulePrime<UInt>& ZModulePrime<UInt>::operator/= (const U& other){
   this->n = ((this->n) * ZModulePrime<UInt>(static_cast<value_type>(other)).inverse())%N;
   return *this;
}

/********************************************************************/
/************* Operator / for same and different types **************/
/********************************************************************/
template<auto UInt>
ZModulePrime<UInt> operator/ (const ZModulePrime<UInt>& lhs, const ZModulePrime<UInt>& rhs){
   return ZModulePrime<UInt>(lhs) /= rhs;
}

template<auto UInt, typename U>
ZModulePrime<UInt> operator/ (const ZModulePrime<UInt>& lhs, const U& rhs){
   return ZModulePrime<UInt>(lhs) /= static_cast<typename ZModulePrime<UInt>::value_type>(rhs);
}
template<auto UInt, typename U>
ZModulePrime<UInt> operator/ (const U& lhs, const ZModulePrime<UInt>& rhs){
   return ZModulePrime<UInt>(static_cast<typename ZModulePrime<UInt>::value_type>(lhs)) /= rhs;
}

// Calculate the inverse of the number in the ring
template<auto UInt>
auto ZModulePrime<UInt>::inverse() const{
   // TODO: Implement Extended Euclidean Algorithm, this one is slow
   value_type ret=1;
   while (((this->n)*ret)%N != 1) ++ret;
   return ret;
}

// Conversion to another prime integer ring (must use static_cast<>())
template<auto UInt> template<auto UInt2>
ZModulePrime<UInt>::operator ZModulePrime<UInt2>() const{
   return ZModulePrime<UInt2>(this->n);
}
