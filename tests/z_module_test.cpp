#include "z_module.hpp"
#define PRIME_CHECK_SUPPORT
#include "z_module_prime.hpp"

#include <iostream>

template<template<auto> typename T, auto UInt1 = 100, auto UInt2 = UInt1*2>
void test_z_module(){
   // Construction
   T<UInt1> a(10), b(20), c(30);

   // Increment and decrement
   ++a;  --a;
   a++;  a--;

   // Unary + and -
   a = +b;
   a = -b;

   // Arithmetic for same type +=, -= and *=
   a += b;
   a -= b;
   a *= b;

   // Arithmetic for PODs types +=, -= and *=
   a += 5;
   a -= 10;
   a *= 2;

   // Comparison for same types ==, !=, <, <=, > and >=
   a == b;
   a != b;
   a < b;
   a <= b;
   a > b;
   a >= b;

   // Comparison for different types ==, !=, <, <=, > and >=
   a == 10;
   a != 10;
   a < 10;
   a <= 10;
   a > 10;
   a >= 10;

   // Castings
   T<UInt2> d(static_cast<decltype(d)>(a));

   // Binary +, - and * operators for same type
   a = b + c;
   a = b - c;
   a = b * c;

   // Binary +, - and * operators for different types
   a = b + 10; a = 10 + b;
   a = b - 5;  a = 5 - b;
   a = b * 2;  a = 2 * b;

   // I/O
   std::cin >> d;
   std::cout << d << std::endl;
}

template<template<auto> typename T, auto UInt1 = 101, auto UInt2 = 211>
void test_z_module_prime(){
   test_z_module<T, UInt1, UInt2>();
}

int main(){
   detail::ZModulePrime<4294967291> z;
   test_z_module<detail::ZModule>();
   //test_z_module_prime<detail::ZModulePrime>();
}