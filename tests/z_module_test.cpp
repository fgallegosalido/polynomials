#include "z_module.hpp"

int main(){
   // Construction
   detail::ZModule<100> a(10), b(20), c(30);

   // Increment and decrement
   ++a;
   --a;
   a++;
   a--;

   // Unary + and -
   b = +a;
   b = -b;

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
   detail::ZModule<200> d(static_cast<decltype(d)>(a));

   // Binary operators for same type +, - and *
   a = b + c;
   a = b - c;
   a = b * c;

   // Binary operators for different types +, - and *
   a = b + 10;
   a = b - 5;
   a = b * 2;

   a = 10 + b;
   a = 5 - b;
   a = 2 * b;

   // I/O
   std::cin >> d;
   std::cout << d << std::endl;
}
