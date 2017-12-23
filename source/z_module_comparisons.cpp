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

/****************************************************************************/
/************* Operators ==, !=, <, <=, > and >= for same type **************/
/****************************************************************************/
template<auto Integral>
constexpr bool operator== (const Z_MODULE_TYPE<Integral>& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   return static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(lhs) ==
            static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(rhs);
}
template<auto Integral>
constexpr bool operator!= (const Z_MODULE_TYPE<Integral>& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   return static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(lhs) !=
            static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(rhs);
}
template<auto Integral>
constexpr bool operator< (const Z_MODULE_TYPE<Integral>& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   return static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(lhs) <
            static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(rhs);
}
template<auto Integral>
constexpr bool operator<= (const Z_MODULE_TYPE<Integral>& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   return static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(lhs) <=
            static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(rhs);
}
template<auto Integral>
constexpr bool operator> (const Z_MODULE_TYPE<Integral>& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   return static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(lhs) >
            static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(rhs);
}
template<auto Integral>
constexpr bool operator>= (const Z_MODULE_TYPE<Integral>& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   return static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(lhs) >=
            static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(rhs);
}


/*****************************************************************************/
/*********** Operators ==, !=, <, <=, > and >= for different type ************/
/*****************************************************************************/
template<auto Integral, typename U>
constexpr bool operator== (const Z_MODULE_TYPE<Integral>& lhs, const U& rhs){
   return lhs == Z_MODULE_TYPE<Integral>(static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(rhs));
}
template<auto Integral, typename U>
constexpr bool operator!= (const Z_MODULE_TYPE<Integral>& lhs, const U& rhs){
   return lhs != Z_MODULE_TYPE<Integral>(static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(rhs));
}
template<auto Integral, typename U>
constexpr bool operator< (const Z_MODULE_TYPE<Integral>& lhs, const U& rhs){
   return lhs < Z_MODULE_TYPE<Integral>(static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(rhs));
}
template<auto Integral, typename U>
constexpr bool operator<= (const Z_MODULE_TYPE<Integral>& lhs, const U& rhs){
   return lhs <= Z_MODULE_TYPE<Integral>(static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(rhs));
}
template<auto Integral, typename U>
constexpr bool operator> (const Z_MODULE_TYPE<Integral>& lhs, const U& rhs){
   return lhs > Z_MODULE_TYPE<Integral>(static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(rhs));
}
template<auto Integral, typename U>
constexpr bool operator>= (const Z_MODULE_TYPE<Integral>& lhs, const U& rhs){
   return lhs >= Z_MODULE_TYPE<Integral>(static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(rhs));
}
/*------------------------------------------*/
template<auto Integral, typename U>
constexpr bool operator== (const U& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   return Z_MODULE_TYPE<Integral>(static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(lhs)) == rhs;
}
template<auto Integral, typename U>
constexpr bool operator!= (const U& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   return Z_MODULE_TYPE<Integral>(static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(lhs)) != rhs;
}
template<auto Integral, typename U>
constexpr bool operator< (const U& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   return Z_MODULE_TYPE<Integral>(static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(lhs)) < rhs;
}
template<auto Integral, typename U>
constexpr bool operator<= (const U& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   return Z_MODULE_TYPE<Integral>(static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(lhs)) <= rhs;
}
template<auto Integral, typename U>
constexpr bool operator> (const U& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   return Z_MODULE_TYPE<Integral>(static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(lhs)) > rhs;
}
template<auto Integral, typename U>
constexpr bool operator>= (const U& lhs, const Z_MODULE_TYPE<Integral>& rhs){
   return Z_MODULE_TYPE<Integral>(static_cast<typename Z_MODULE_TYPE<Integral>::value_type>(lhs)) >= rhs;
}
