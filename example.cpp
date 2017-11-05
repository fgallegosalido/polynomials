#define UNICODE_SUPPORT
#include "polynomial.hpp"
#include <iostream>
#include <vector>

typedef polynomial_float64 polynomial;

int main(){
   std::vector<polynomial::value_type> v = {2, 2, 1};
   polynomial p1(v.begin(), v.end());
   polynomial p2 = {1, 1};
   polynomial p10 = {1};
   auto p3 = p1+p2;
   auto p4 = p1-p2;
   auto p5 = p1*p2;
   auto p6 = p1/p2;
   auto p7 = p1%p2;
   auto p8 = p2/p10;
   auto p9 = p2%p10;

   std::cout << p3 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << p4 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << p5 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << p6 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << p7 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << p8 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << p9 << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << p8.evaluate_at(2) << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << gcd(p1, p2) << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << lcm(p1, p2) << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << differentiate(p5) << std::endl;
   std::cout << "-----------" << std::endl;
   std::cout << ((p5 == lcm(p1,p2))?"True":"False") << std::endl;
}
