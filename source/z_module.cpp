template<auto UInt>
constexpr std::string ZModule<UInt>::name(){
   return aux::unicode::integer_set_symbol + aux::subindex_string(N);
}


/************* Constructor *************/
template<auto UInt>
constexpr ZModule<UInt>::ZModule (const value_type& zm) : n(zm%N){}


/*************************************************************/
/************** Increment/Decrement operators ****************/
/*************************************************************/
template<auto UInt>
constexpr ZModule<UInt>& ZModule<UInt>::operator++ (){
   n = (n+1)%N;
   return *this;
}
template<auto UInt>
constexpr ZModule<UInt>& ZModule<UInt>::operator-- (){
   n = (n==0)?N-1:n-1;
   return *this;
}
/*-------------------------------------*/
template<auto UInt>
constexpr ZModule<UInt> ZModule<UInt>::operator++ (int){
   ZModule<UInt> ret(*this);
   ++(*this);
   return ret;
}
template<auto UInt>
constexpr ZModule<UInt> ZModule<UInt>::operator-- (int){
   ZModule<UInt> ret(*this);
   --(*this);
   return ret;
}


/***************************************************************/
/************* Operators +=, -=, *= for same type **************/
/***************************************************************/
template<auto UInt>
constexpr ZModule<UInt>& ZModule<UInt>::operator+= (const ZModule<UInt>& zm){
   n = (n+zm.n)%N;
   return *this;
}
template<auto UInt>
constexpr ZModule<UInt>& ZModule<UInt>::operator-= (const ZModule<UInt>& zm){
   n = (n+N-zm.n)%N;
   return *this;
}
template<auto UInt>
constexpr ZModule<UInt>& ZModule<UInt>::operator*= (const ZModule<UInt>& zm){
   n = (n*zm.n)%N;
   return *this;
}


/*****************************************************************************/
/************ Operators +=, -=, *= for other types compatibility *************/
/*****************************************************************************/
template<auto UInt> template<typename U>
constexpr ZModule<UInt>& ZModule<UInt>::operator+= (const U& other){
   n = (n+static_cast<value_type>(other))%N;
   return *this;
}
template<auto UInt> template<typename U>
constexpr ZModule<UInt>& ZModule<UInt>::operator-= (const U& other){
   n = (n+N-static_cast<value_type>(other))%N;
   return *this;
}
template<auto UInt> template<typename U>
constexpr ZModule<UInt>& ZModule<UInt>::operator*= (const U& other){
   n = (n*static_cast<value_type>(other))%N;
   return *this;
}


/*********************************************************************/
/************** Castings to value_type or other ZModule **************/
/*********************************************************************/
template<auto UInt>
constexpr ZModule<UInt>::operator ZModule<UInt>::value_type() const{
   return n;
}
template<auto UInt> template<auto UInt2>
constexpr ZModule<UInt>::operator ZModule<UInt2>() const{
   return ZModule<UInt2>(n);
}
