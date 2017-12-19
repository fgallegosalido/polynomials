#define PRIME_CHECK_SUPPORT
#define UNICODE_SUPPORT
#include "z_module.hpp"
#include "z_module_prime.hpp"

#include <iostream>

// Function to check correctness of ZModule class
template<template<auto> typename T, auto UInt1 = 1234, auto UInt2 = UInt1*2>
void test_z_module(){
   // Name of the class
   std::cout << T<UInt1>::name() << std::endl;

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
   a == 10; 10 == a;
   a != 10; 10 != a;
   a < 10;  10 < a;
   a <= 10; 10 <= a;
   a > 10;  10 > a;
   a >= 10; 10 >= a;

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
   T<UInt2> e;
   std::cout << "Testing input....\n";
   std::cin >> e;
   std::cout << "The typed number is " << e << " in " << T<UInt2>::name() << std::endl;
}

// Function to check correctness of ZModulePrime class
template<template<auto> typename T, auto UInt1 = 1367, auto UInt2 = 5683>
void test_z_module_prime(){
   test_z_module<T, UInt1, UInt2>();

   T<UInt1> a(10), b(20), c(30);

   // Unary /= operator for same and different types
   a /= b;
   a /= 5;

   // Casting
   T<UInt2> d(static_cast<decltype(d)>(a));

   // Binary / operator for same and different types
   a = b / c;
   a = b / 10;
   a = 10 / b;

   // Exception test
   std::cout << "\n/************************************/\n"
             <<   " Testing exception for divide by zero"
             << "\n/************************************/\n"
             << std::flush;
   a /= 0;
}

int main(){
   test_z_module<detail::ZModule>();
   test_z_module_prime<detail::ZModulePrime>();
}
