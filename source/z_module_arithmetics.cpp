// This macros will define the overloadings for ZModulePrime only if
// the header is included. Otherwise, only the ones for ZModule
// will be defined.
#ifdef INCLUDED_Z_MODULE_PRIME
   #undef Z_MODULE_TYPE
   #define Z_MODULE_TYPE ZModulePrime
#else
   #undef Z_MODULE_TYPE
   #define Z_MODULE_TYPE ZModule
#endif

/***************************************************/
/*********** Unary + and - operators ***************/
/***************************************************/
template<auto UInt>
constexpr Z_MODULE_TYPE<UInt> operator+ (const Z_MODULE_TYPE<UInt>& rhs){
   return Z_MODULE_TYPE<UInt>(rhs);
}
template<auto UInt>
constexpr Z_MODULE_TYPE<UInt> operator- (const Z_MODULE_TYPE<UInt>& rhs){
   return Z_MODULE_TYPE<UInt>(Z_MODULE_TYPE<UInt>::N -
      static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(rhs));
}


/************************************************************************/
/*************** Binary operators +, -, * for same type *****************/
/************************************************************************/
template<auto UInt>
constexpr Z_MODULE_TYPE<UInt> operator+ (const Z_MODULE_TYPE<UInt>& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   Z_MODULE_TYPE<UInt> ret(lhs);
   ret += rhs;
   return ret;
}
template<auto UInt>
constexpr Z_MODULE_TYPE<UInt> operator- (const Z_MODULE_TYPE<UInt>& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   Z_MODULE_TYPE<UInt> ret(lhs);
   ret -= rhs;
   return ret;
}
template<auto UInt>
constexpr Z_MODULE_TYPE<UInt> operator* (const Z_MODULE_TYPE<UInt>& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   Z_MODULE_TYPE<UInt> ret(lhs);
   ret *= rhs;
   return ret;
}


/*****************************************************************************/
/*************** Binary operators +, -, * for different types ****************/
/*****************************************************************************/
template<auto UInt, typename U>
constexpr Z_MODULE_TYPE<UInt> operator+ (const Z_MODULE_TYPE<UInt>& lhs, const U& rhs){
   Z_MODULE_TYPE<UInt> ret(lhs);
   ret += rhs;
   return ret;
}
template<auto UInt, typename U>
constexpr Z_MODULE_TYPE<UInt> operator- (const Z_MODULE_TYPE<UInt>& lhs, const U& rhs){
   Z_MODULE_TYPE<UInt> ret(lhs);
   ret -= rhs;
   return ret;
}
template<auto UInt, typename U>
constexpr Z_MODULE_TYPE<UInt> operator* (const Z_MODULE_TYPE<UInt>& lhs, const U& rhs){
   Z_MODULE_TYPE<UInt> ret(lhs);
   ret *= rhs;
   return ret;
}
/*------------------------------------------*/
template<auto UInt, typename U>
constexpr Z_MODULE_TYPE<UInt> operator+ (const U& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   Z_MODULE_TYPE<UInt> ret(rhs);
   ret += lhs;
   return ret;
}
template<auto UInt, typename U>
constexpr Z_MODULE_TYPE<UInt> operator- (const U& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   Z_MODULE_TYPE<UInt> ret(rhs);
   ret -= lhs;
   return -ret;
}
template<auto UInt, typename U>
constexpr Z_MODULE_TYPE<UInt> operator* (const U& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   Z_MODULE_TYPE<UInt> ret(rhs);
   ret *= lhs;
   return ret;
}
