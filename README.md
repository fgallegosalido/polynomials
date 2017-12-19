# polynomials
A polynomial library for C++17 and beyond.

## Description
This is a header-only library (which means that nothing has to be built). It implements two fundamental types:

  * A Polynomial class.
  * Two number types that can work for the Polynomial class:
    * Z-module numbers.
    * Fields of Z-module (prime Z-modules).

#### Polynomial
This class tries to emulate the operations of a polynomial over a ring (there will be a specialization for fields). It's implemented as a template class, where the template parameter indicates the ring where the coeffcientes are defined (in future releases, there will be also a second parameter for the return type of the evaluation).

It defines the arithmetic operations of a polynomial: addition, substraction, product, division and reminder. It also defines some other transformations over a polynomial like differentiation, integration, exponentiation and some others.

#### Z-module
This class emulates how modular numbers work. It accepts only one template parameter, which is the cardinal of the Z-module ring. There's a specialization for prime cardinals, so the Z-module can act as a field (which implies more operations).

It implements the basic arithmetic for Z-module: addition, substraction and product. For prime cardinal Z-modules, the division is also defined (multiplying by the inverse of the number).

The core of this type is mostly complete (excluding debug and future additions).

There's a compile time primality test for Z-module with prime cardinality in case you want to ensure the field properties. Keep in mind that this test can imply that compilation time increases. This can be customized with macros (see **Customization** for more information).

## Use
To use this library, you can just download or clone the repository and include the headers where necessary (assuming that the directory is in your project directory).

Everything is under the `detail` namespace, so you can prefix `detail::` to the types, or just use `using namespace detail;` for simplicity. You can use it like this:

```cpp
#include "polynomials/include/polynomial.hpp"
#include "polynomials/include/z_module.hpp"
#include <iostream>
#include <vector>

int main(){
  detail::Polynomial<double> pol = {1.5, 0, -8, 9}; // Creates the polinomial 9x^3 - 8x^2 + 1.5 over the real numbers (double)
  std::vector<double> v = {1, -2, 3};
  detail::Polynomial<double> pol2(v.begin(), v.end()); // Construct with range constructor

  std::cout << "Testing addition :\n("
            << pol << ") + (" << pol2 << ") = " << pol + pol2 << std::endl;

  detail::Polynomial<detail::ZModule<7>> p_mod = {1, 2, 3, 8}; // In Z-module, this is x^3 + 3x^2 + 2x + 1
  detail::Polynomial<detail::ZModule<7>> p_mod2 = {v.begin(), v.end()};

  std::cout << p_mod*p_mod2 << std::endl;
}
```

This is just a minimal example. More complex examples will be provided in the *tests* directory.

## Customization
This whole library comes with some macros, so you can decide which types you want in your program and some other options. These are the macros you can define in order filter what you want to compile:

  1. **UNICODE_SUPPORT**: This will allow the output to use unicode characters (don't use it if your shell doesn't support unicode, which most do).
  2. **Z_MODULE_SUPPORT**: This will automatically let you use Z-module types and provides some handy typedefs.
  3. **RATIONAL__SUPPORT**: This will let you rational numbers, provided by [Boost.Rational](http://www.boost.org/doc/libs/1_66_0/libs/rational/) (requires boost library).
  4. **MULTIPRECISION_SUPPORT**: Allows you to use multiprecision numbers, provided by [Boost.Multiprecision](http://www.boost.org/doc/libs/1_66_0/libs/multiprecision/doc/html/index.html) (requires boost library).
  5. **FAST_MULTIPRECISION_SUPPORT**: Uses [GMP](https://gmplib.org/) implementation for multiprecision (requires boost library and gmp library), which is (probably) the fastest library for multiprecision.
  6. **PRIME_CHECK_SUPPORT**: Provides a compile-time primality test to ensure a Z-module field has prime cardinality. Otherwise, a `static_assert` will be thrown, leading to a compile error. If not defined, the test will always evaluate true.

Some macros are there because of external libraries dependencies, so if you don't have those libraries installed, you can tell the compiler not to compile those types.

**Note**: In Ubuntu 16.04 LTS, the default version for boost is 1.58, which has a bug related to the multiprecision library. Use a later version.

In order to use the macros, just include `#define` directives at the top of your program, like this:

```cpp
#define UNICODE_SUPPORT
#define MULTIPRECISION_SUPPORT
#define PRIME_CHECK_SUPPORT

#include "polynomials/include/polynomial.hpp"
#include "polynomials/inlcude/z_module_prime.hpp"

.....
```

## Extra
For the polynomial class, there are some handy typedefs that can help you to write cleaner code. These are all the typedefs provided (under the `detail` namespace):

```cpp
// Typedefs for coefficients with integer types
typedef Polynomial<std::int8_t> polynomial_int8;
typedef Polynomial<std::int16_t> polynomial_int16;
typedef Polynomial<std::int32_t> polynomial_int32;
typedef Polynomial<std::int64_t> polynomial_int64;

// Typedefs for coefficients in floating point (real numbers)
typedef Polynomial<float> polynomial_float;
typedef Polynomial<double> polynomial_double;
typedef Polynomial<long double> polynomial_long_double;

// Typedefs for extra types (boost::rational and std::complex)
typedef Polynomial<boost::rational<std::int8_t>> polynomial_rational_int8;
typedef Polynomial<boost::rational<std::int16_t>> polynomial_rational_int16;
typedef Polynomial<boost::rational<std::int32_t>> polynomial_rational_int32;
typedef Polynomial<boost::rational<std::int64_t>> polynomial_rational_int64;

typedef Polynomial<std::complex<float>> polynomial_complex_float;
typedef Polynomial<std::complex<double>> polynomial_complex_double;
typedef Polynomial<std::complex<long double>> polynomial_complex_long_double;

/* Typedefs for multiprecision polynomial using boost multiprecision
 * library or fast multiprecision using gmp library
 *
 * The types multiprecision_{int,rational,float} are defined depending
 * if it uses boost implementation or gmp one.
 */
typedef Polynomial<multiprecision_int> polynomial_multiprecision_int;
typedef Polynomial<multiprecision_rational> polynomial_multiprecision_rational;
typedef Polynomial<multiprecision_float> polynomial_multiprecision_float;
typedef Polynomial<std::complex<multiprecision_float>> polynomial_multiprecision_complex;

// Typedefs for Z-module types
template<auto N>
using polynomial_modular = Polynomial<ZModule<N>>;
template<auto N>
using polynomial_modular_prime = Polynomial<ZModulePrime<N>>;
```

## Some notes
The polynomial class is still under work in progress, so can be full of bugs and inconsistencies (pull request are open to everybody). Use it carefully.

However, the Z-module type (the ring and the field for prime cardinality) is almost finished, lacking some optimizations and debugging.
