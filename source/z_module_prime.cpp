/************* Constructor *************/
template<auto UInt, typename Dummy>
ZModulePrime<UInt, Dummy>::ZModulePrime (const value_type& zm) : ZModule<UInt>(zm){}

/*********************************************************************/
/************* Operator /= for same and different types **************/
/*********************************************************************/
template<auto UInt, typename Dummy>
ZModulePrime<UInt, Dummy>& ZModulePrime<UInt, Dummy>::operator/= (const ZModulePrime<UInt, Dummy>& zm){
   this->n = ((this->n)*inverse(zm))%N;
   return *this;
}
template<auto UInt, typename Dummy> template<typename U>
ZModulePrime<UInt, Dummy>& ZModulePrime<UInt, Dummy>::operator/= (const U& other){
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
template<auto UInt, typename Dummy>
auto ZModulePrime<UInt, Dummy>::inverse() const{
   // TODO: Implement Extended Euclidean Algorithm
   value_type ret=1;
   while (((this->n)*ret)%N != 1) ++ret;
   return ret;
}

// Conversion to another prime integer ring (must use static_cast<>())
template<auto UInt, typename Dummy> template<auto UInt2>
ZModulePrime<UInt, Dummy>::operator ZModulePrime<UInt2>() const{
   return ZModulePrime<UInt2>(this->n);
}
