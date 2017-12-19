/************* Constructor *************/
template<auto UInt>
ZModule<UInt>::ZModule (const value_type& zm) : n(zm%N){}


/*************************************************************/
/************** Increment/Decrement operators ****************/
/*************************************************************/
template<auto UInt>
ZModule<UInt>& ZModule<UInt>::operator++ (){
   n = (n+1)%N;
   return *this;
}
template<auto UInt>
ZModule<UInt>& ZModule<UInt>::operator-- (){
   n = (n==0)?N-1:n-1;
   return *this;
}
/*-------------------------------------*/
template<auto UInt>
ZModule<UInt> ZModule<UInt>::operator++ (int){
   ZModule<UInt> ret(*this);
   ++(*this);
   return ret;
}
template<auto UInt>
ZModule<UInt> ZModule<UInt>::operator-- (int){
   ZModule<UInt> ret(*this);
   --(*this);
   return ret;
}


/***************************************************************/
/************* Operators +=, -=, *= for same type **************/
/***************************************************************/
template<auto UInt>
ZModule<UInt>& ZModule<UInt>::operator+= (const ZModule<UInt>& zm){
   n = (n+zm.n)%N;
   return *this;
}
template<auto UInt>
ZModule<UInt>& ZModule<UInt>::operator-= (const ZModule<UInt>& zm){
   n = (n+N-zm.n)%N;
   return *this;
}
template<auto UInt>
ZModule<UInt>& ZModule<UInt>::operator*= (const ZModule<UInt>& zm){
   n = (n*zm.n)%N;
   return *this;
}


/*****************************************************************************/
/************ Operators +=, -=, *= for other types compatibility *************/
/*****************************************************************************/
template<auto UInt> template<typename U>
ZModule<UInt>& ZModule<UInt>::operator+= (const U& other){
   n = (n+static_cast<value_type>(other))%N;
   return *this;
}
template<auto UInt> template<typename U>
ZModule<UInt>& ZModule<UInt>::operator-= (const U& other){
   n = (n+N-static_cast<value_type>(other))%N;
   return *this;
}
template<auto UInt> template<typename U>
ZModule<UInt>& ZModule<UInt>::operator*= (const U& other){
   n = (n*static_cast<value_type>(other))%N;
   return *this;
}


/****************************************************************************/
/************* Operators ==, !=, <, <=, > and >= for same type **************/
/****************************************************************************/
template<auto UInt>
bool ZModule<UInt>::operator== (const ZModule<UInt>& zm) const{
   return n == zm.n;
}
template<auto UInt>
bool ZModule<UInt>::operator!= (const ZModule<UInt>& zm) const{
   return n != zm.n;
}
template<auto UInt>
bool ZModule<UInt>::operator< (const ZModule<UInt>& zm) const{
   return n < zm.n;
}
template<auto UInt>
bool ZModule<UInt>::operator<= (const ZModule<UInt>& zm) const{
   return n <= zm.n;
}
template<auto UInt>
bool ZModule<UInt>::operator> (const ZModule<UInt>& zm) const{
   return n > zm.n;
}
template<auto UInt>
bool ZModule<UInt>::operator>= (const ZModule<UInt>& zm) const{
   return n >= zm.n;
}


/*****************************************************************************/
/*********** Operators ==, !=, <, <=, > and >= for different type ************/
/*****************************************************************************/
template<auto UInt> template<typename U>
bool ZModule<UInt>::operator== (const U& other) const{
   return n == static_cast<value_type>(other);
}
template<auto UInt> template<typename U>
bool ZModule<UInt>::operator!= (const U& other) const{
   return n != static_cast<value_type>(other);
}
template<auto UInt> template<typename U>
bool ZModule<UInt>::operator< (const U& other) const{
   return n < static_cast<value_type>(other);
}
template<auto UInt> template<typename U>
bool ZModule<UInt>::operator<= (const U& other) const{
   return n <= static_cast<value_type>(other);
}
template<auto UInt> template<typename U>
bool ZModule<UInt>::operator> (const U& other) const{
   return n > static_cast<value_type>(other);
}
template<auto UInt> template<typename U>
bool ZModule<UInt>::operator>= (const U& other) const{
   return n >= static_cast<value_type>(other);
}


/*********************************************************************/
/************** Castings to value_type or other ZModule **************/
/*********************************************************************/
template<auto UInt>
ZModule<UInt>::operator ZModule<UInt>::value_type() const{
   return n;
}
template<auto UInt> template<auto UInt2>
ZModule<UInt>::operator ZModule<UInt2>() const{
   return ZModule<UInt2>(n);
}
