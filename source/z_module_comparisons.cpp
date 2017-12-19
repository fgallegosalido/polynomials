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
template<auto UInt>
bool operator== (const Z_MODULE_TYPE<UInt>& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   return static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(lhs) ==
            static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(rhs);
}
template<auto UInt>
bool operator!= (const Z_MODULE_TYPE<UInt>& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   return static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(lhs) !=
            static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(rhs);
}
template<auto UInt>
bool operator< (const Z_MODULE_TYPE<UInt>& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   return static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(lhs) <
            static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(rhs);
}
template<auto UInt>
bool operator<= (const Z_MODULE_TYPE<UInt>& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   return static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(lhs) <=
            static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(rhs);
}
template<auto UInt>
bool operator> (const Z_MODULE_TYPE<UInt>& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   return static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(lhs) >
            static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(rhs);
}
template<auto UInt>
bool operator>= (const Z_MODULE_TYPE<UInt>& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   return static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(lhs) >=
            static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(rhs);
}


/*****************************************************************************/
/*********** Operators ==, !=, <, <=, > and >= for different type ************/
/*****************************************************************************/
template<auto UInt, typename U>
bool operator== (const Z_MODULE_TYPE<UInt>& lhs, const U& rhs){
   return lhs == Z_MODULE_TYPE<UInt>(static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(rhs));
}
template<auto UInt, typename U>
bool operator!= (const Z_MODULE_TYPE<UInt>& lhs, const U& rhs){
   return lhs != Z_MODULE_TYPE<UInt>(static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(rhs));
}
template<auto UInt, typename U>
bool operator< (const Z_MODULE_TYPE<UInt>& lhs, const U& rhs){
   return lhs < Z_MODULE_TYPE<UInt>(static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(rhs));
}
template<auto UInt, typename U>
bool operator<= (const Z_MODULE_TYPE<UInt>& lhs, const U& rhs){
   return lhs <= Z_MODULE_TYPE<UInt>(static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(rhs));
}
template<auto UInt, typename U>
bool operator> (const Z_MODULE_TYPE<UInt>& lhs, const U& rhs){
   return lhs > Z_MODULE_TYPE<UInt>(static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(rhs));
}
template<auto UInt, typename U>
bool operator>= (const Z_MODULE_TYPE<UInt>& lhs, const U& rhs){
   return lhs >= Z_MODULE_TYPE<UInt>(static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(rhs));
}
/*------------------------------------------*/
template<auto UInt, typename U>
bool operator== (const U& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   return Z_MODULE_TYPE<UInt>(static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(lhs)) == rhs;
}
template<auto UInt, typename U>
bool operator!= (const U& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   return Z_MODULE_TYPE<UInt>(static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(lhs)) != rhs;
}
template<auto UInt, typename U>
bool operator< (const U& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   return Z_MODULE_TYPE<UInt>(static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(lhs)) < rhs;
}
template<auto UInt, typename U>
bool operator<= (const U& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   return Z_MODULE_TYPE<UInt>(static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(lhs)) <= rhs;
}
template<auto UInt, typename U>
bool operator> (const U& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   return Z_MODULE_TYPE<UInt>(static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(lhs)) > rhs;
}
template<auto UInt, typename U>
bool operator>= (const U& lhs, const Z_MODULE_TYPE<UInt>& rhs){
   return Z_MODULE_TYPE<UInt>(static_cast<typename Z_MODULE_TYPE<UInt>::value_type>(lhs)) >= rhs;
}
