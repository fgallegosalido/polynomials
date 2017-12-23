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
template<auto Integral>
constexpr Z_MODULE_TYPE<Integral> operator+ (const Z_MODULE_TYPE<Integral>& rhs){
   return Z_MODULE_TYPE<Integral>(rhs);
}
template<auto Integral>
constexpr Z_MODULE_TYPE<Integral> operator- (const Z_MODULE_TYPE<Integral>& rhs){
   return Z_MODULE_TYPE<Integral>(Z_MODULE_TYPE<Integral>::N -
      static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(rhs));
}


/************************************************************************/
/*************** Binary operators +, -, * for same type *****************/
/************************************************************************/
template<auto Integral>
constexpr Z_MODULE_TYPE<Integral> operator+ (const Z_MODULE_TYPE<Integral>& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   Z_MODULE_TYPE<Integral> ret(lhs);
   ret += rhs;
   return ret;
}
template<auto Integral>
constexpr Z_MODULE_TYPE<Integral> operator- (const Z_MODULE_TYPE<Integral>& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   Z_MODULE_TYPE<Integral> ret(lhs);
   ret -= rhs;
   return ret;
}
template<auto Integral>
constexpr Z_MODULE_TYPE<Integral> operator* (const Z_MODULE_TYPE<Integral>& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   Z_MODULE_TYPE<Integral> ret(lhs);
   ret *= rhs;
   return ret;
}


/*****************************************************************************/
/*************** Binary operators +, -, * for different types ****************/
/*****************************************************************************/
template<auto Integral, typename U>
constexpr Z_MODULE_TYPE<Integral> operator+ (const Z_MODULE_TYPE<Integral>& lhs, const U& rhs){
   Z_MODULE_TYPE<Integral> ret(lhs);
   ret += rhs;
   return ret;
}
template<auto Integral, typename U>
constexpr Z_MODULE_TYPE<Integral> operator- (const Z_MODULE_TYPE<Integral>& lhs, const U& rhs){
   Z_MODULE_TYPE<Integral> ret(lhs);
   ret -= rhs;
   return ret;
}
template<auto Integral, typename U>
constexpr Z_MODULE_TYPE<Integral> operator* (const Z_MODULE_TYPE<Integral>& lhs, const U& rhs){
   Z_MODULE_TYPE<Integral> ret(lhs);
   ret *= rhs;
   return ret;
}
/*------------------------------------------*/
template<auto Integral, typename U>
constexpr Z_MODULE_TYPE<Integral> operator+ (const U& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   Z_MODULE_TYPE<Integral> ret(rhs);
   ret += lhs;
   return ret;
}
template<auto Integral, typename U>
constexpr Z_MODULE_TYPE<Integral> operator- (const U& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   Z_MODULE_TYPE<Integral> ret(rhs);
   ret -= lhs;
   return -ret;
}
template<auto Integral, typename U>
constexpr Z_MODULE_TYPE<Integral> operator* (const U& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   Z_MODULE_TYPE<Integral> ret(rhs);
   ret *= lhs;
   return ret;
}
