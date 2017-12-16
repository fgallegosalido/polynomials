#define UNICODE_SUPPORT
#define MULTIPRECISION_SUPPORT
//#define FAST_MULTIPRECISION_SUPPORT

#include "polynomial.hpp"
#include <iostream>
#include <vector>

#include <boost/version.hpp>

int main(){
   std::cout << "Using Boost "
          << BOOST_VERSION / 100000     << "."  // major version
          << BOOST_VERSION / 100 % 1000 << "."  // minor version
          << BOOST_VERSION % 100                // patch level
          << std::endl;

   using namespace std::complex_literals;
   detail::polynomial_complex_double c = {{0,1}, 1, {0,-1}};

   std::vector<detail::polynomial_double::value_type> v = {2, 2, 1};
   detail::polynomial_double p1(v.begin(), v.end());
   detail::polynomial_double p2 = {1, 1};
   auto p3 = p1+p2;
   auto p4 = p1-p2;
   auto p5 = p1*p2;
   auto p6 = p1/p2;
   auto p7 = p1%p2;

   std::cout << "-----------" << std::endl;
   std::cout << "p1: " << p1 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << "p2: " << p2 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << "Addition: " << p3 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << "Substraction: " << p4 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << "Product: " << p5 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << "Division: " << p6 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << "Reminder: " << p7 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << "Evaluation: " << p1(2) << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << "GCD: " << detail::gcd(p1, p2) << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << "LCM: " << detail::lcm(p1, p2) << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << "Differentiate: " << detail::differentiate(p5) << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << std::boolalpha << "Equal comparison: " << (p5 == lcm(p1,p2)) << std::endl;
   std::cout << "-----------" << std::endl;

   detail::ZModulePrime<98773> x1(346), x2(300);
   std::cout << "Inverse of " << ++x1 << " module 98773" << ": " << x1.inverse() << std::endl;
   std::cout << "-----------" << std::endl;

   detail::ZModulePrime<33> y = static_cast<detail::ZModulePrime<33>>(x1);
   std::cout << "Inverse of " << y << " module 33" << ": " << y.inverse() << std::endl;
   std::cout << "-----------" << std::endl;

   x1 = x1 + x2;
}
