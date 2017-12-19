// This macros will define the overloadings for ZModulePrime only if
// the header is included. Otherwise, only the ones for ZModule
// will be defined.
#ifdef INCLUDED_Z_MODULE_PRIME
   #undef Z_MODULE_RETURN
   #define Z_MODULE_RETURN ZModulePrime
#else
   #define Z_MODULE_RETURN ZModule
#endif

/***************************************************/
/*********** Unary + and - operators ***************/
/***************************************************/
template<auto UInt>
Z_MODULE_RETURN<UInt> operator+ (const Z_MODULE_RETURN<UInt>& rhs){
   return Z_MODULE_RETURN<UInt>(rhs);
}
template<auto UInt>
Z_MODULE_RETURN<UInt> operator- (const Z_MODULE_RETURN<UInt>& rhs){
   return Z_MODULE_RETURN<UInt>(Z_MODULE_RETURN<UInt>::N -
      static_cast<typename Z_MODULE_RETURN<UInt>::value_type>(rhs));
}


/************************************************************************/
/*************** Binary operators +, -, * for same type *****************/
/************************************************************************/
template<auto UInt>
Z_MODULE_RETURN<UInt> operator+ (const Z_MODULE_RETURN<UInt>& lhs, const Z_MODULE_RETURN<UInt>& rhs){
   Z_MODULE_RETURN<UInt> ret(lhs);
   ret += rhs;
   return ret;
}
template<auto UInt>
Z_MODULE_RETURN<UInt> operator- (const Z_MODULE_RETURN<UInt>& lhs, const Z_MODULE_RETURN<UInt>& rhs){
   Z_MODULE_RETURN<UInt> ret(lhs);
   ret -= rhs;
   return ret;
}
template<auto UInt>
Z_MODULE_RETURN<UInt> operator* (const Z_MODULE_RETURN<UInt>& lhs, const Z_MODULE_RETURN<UInt>& rhs){
   Z_MODULE_RETURN<UInt> ret(lhs);
   ret *= rhs;
   return ret;
}


/*****************************************************************************/
/*************** Binary operators +, -, * for different types ****************/
/*****************************************************************************/
template<auto UInt, typename U>
Z_MODULE_RETURN<UInt> operator+ (const Z_MODULE_RETURN<UInt>& lhs, const U& rhs){
   Z_MODULE_RETURN<UInt> ret(lhs);
   ret += rhs;
   return ret;
}
template<auto UInt, typename U>
Z_MODULE_RETURN<UInt> operator- (const Z_MODULE_RETURN<UInt>& lhs, const U& rhs){
   Z_MODULE_RETURN<UInt> ret(lhs);
   ret -= rhs;
   return ret;
}
template<auto UInt, typename U>
Z_MODULE_RETURN<UInt> operator* (const Z_MODULE_RETURN<UInt>& lhs, const U& rhs){
   Z_MODULE_RETURN<UInt> ret(lhs);
   ret *= rhs;
   return ret;
}
/*------------------------------------------*/
template<auto UInt, typename U>
Z_MODULE_RETURN<UInt> operator+ (const U& lhs, const Z_MODULE_RETURN<UInt>& rhs){
   Z_MODULE_RETURN<UInt> ret(rhs);
   ret += lhs;
   return ret;
}
template<auto UInt, typename U>
Z_MODULE_RETURN<UInt> operator- (const U& lhs, const Z_MODULE_RETURN<UInt>& rhs){
   Z_MODULE_RETURN<UInt> ret(rhs);
   ret -= lhs;
   return -ret;
}
template<auto UInt, typename U>
Z_MODULE_RETURN<UInt> operator* (const U& lhs, const Z_MODULE_RETURN<UInt>& rhs){
   Z_MODULE_RETURN<UInt> ret(rhs);
   ret *= lhs;
   return ret;
}
