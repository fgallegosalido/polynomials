template<auto Integral>
constexpr std::string ZModule<Integral>::name(){
   return aux::unicode::integer_set_symbol + aux::subindex_string(N);
}


/************* Constructor *************/
template<auto Integral>
constexpr ZModule<Integral>::ZModule (const value_type& zm){
   if (zm < 0){
      n = N - (-zm)%N;
   }
   else{
      n = zm%N;
   }
}


/*************************************************************/
/************** Increment/Decrement operators ****************/
/*************************************************************/
template<auto Integral>
constexpr ZModule<Integral>& ZModule<Integral>::operator++ (){
   n = (n+1)%N;
   return *this;
}
template<auto Integral>
constexpr ZModule<Integral>& ZModule<Integral>::operator-- (){
   n = (n==0)?N-1:n-1;
   return *this;
}
/*-------------------------------------*/
template<auto Integral>
constexpr ZModule<Integral> ZModule<Integral>::operator++ (int){
   ZModule<Integral> ret(*this);
   ++(*this);
   return ret;
}
template<auto Integral>
constexpr ZModule<Integral> ZModule<Integral>::operator-- (int){
   ZModule<Integral> ret(*this);
   --(*this);
   return ret;
}


/***************************************************************/
/************* Operators +=, -=, *= for same type **************/
/***************************************************************/
template<auto Integral>
constexpr ZModule<Integral>& ZModule<Integral>::operator+= (const ZModule<Integral>& zm){
   n = (n+zm.n)%N;
   return *this;
}
template<auto Integral>
constexpr ZModule<Integral>& ZModule<Integral>::operator-= (const ZModule<Integral>& zm){
   n = (n+N-zm.n)%N;
   return *this;
}
template<auto Integral>
constexpr ZModule<Integral>& ZModule<Integral>::operator*= (const ZModule<Integral>& zm){
   n = (n*zm.n)%N;
   return *this;
}


/*****************************************************************************/
/************ Operators +=, -=, *= for other types compatibility *************/
/*****************************************************************************/
template<auto Integral> template<typename U>
constexpr ZModule<Integral>& ZModule<Integral>::operator+= (const U& other){
   n = (n+static_cast<value_type>(other))%N;
   return *this;
}
template<auto Integral> template<typename U>
constexpr ZModule<Integral>& ZModule<Integral>::operator-= (const U& other){
   n = (n+N-static_cast<value_type>(other))%N;
   return *this;
}
template<auto Integral> template<typename U>
constexpr ZModule<Integral>& ZModule<Integral>::operator*= (const U& other){
   n = (n*static_cast<value_type>(other))%N;
   return *this;
}


/*********************************************************************/
/************** Castings to value_type or other ZModule **************/
/*********************************************************************/
template<auto Integral> template<typename T>
constexpr ZModule<Integral>::operator T() const{
   return T(n);
}
template<auto Integral> template<auto Integral2>
constexpr ZModule<Integral>::operator ZModule<Integral2>() const{
   return ZModule<Integral2>(n);
}
