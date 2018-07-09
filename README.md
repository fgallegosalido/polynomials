# polynomials
A polynomial library for C++17 and beyond.

## Description
This is a header-only library (which means that nothing has to be built). It doesn't have any dependecies (the tests make use of *Boost.Test*, so the boost library is needed in order to execute them).

This class tries to emulate the operations of a polynomial over a field (for the moment, only real and complex numbers). It's implemented as a template class, where the template parameter indicates the underlined type where the coefficients are defined (floating-point types for real numbers and `std::complex` for complex numbers).

It defines the arithmetic operations of a polynomial: addition, substraction, product, division and reminder. It also defines some other transformations over a polynomial like differentiation, integration, exponentiation and some others.

It comes with free functions that let you create polynomials in different ways (interpolation, by roots, taylor...) and a function to find the roots of a polynomial (still under work in progress due to inconsistent results for higher-degree polynomials).

## Use
In the future, a full documentation will be provided to make it easier to use this library.

To use this library, you can just download or clone the repository and include the headers where necessary (assuming that the directory is in your project directory).

Everything is under the `fgs` namespace, so you must prefix `fgs::` to the types, or just use `using namespace fgs;` for simplicity (namespaces inside `fgs` should be ignored, as they only have auxiliar fuctions). You can use it like this:

```cpp
#include "polynomial.hpp"

#include <iostream>
#include <vector>

int main(){
  // Creates the polinomial 9x^3 - 8x^2 + 1.5 over the real numbers (double)
  fgs::Polynomial<double> pol = {1.5, 0, -8, 9};

  std::vector<double> v = {1, -2, 3};
  fgs::Polynomial<double> pol2(v.begin(), v.end()); // Construct with range constructor

  std::cout << "Testing addition :\n("
            << pol << ") + (" << pol2 << ") = " << pol + pol2 << std::endl;

  // Creates a polynomial with the roots provided as a list
  fgs::polynomial_double p = fgs::make_polynomial_by_roots({1.0, 0.0, -1.0});
}
```

This is just a minimal example. More complex examples will be provided in the *tests* directory, or just check the *examle.cpp* file (in the future there will be a whole directory full of examples).

## Customization
This whole library comes with some macros, so you can decide which types you want in your program and some other options. These are the macros you can define in order to filter what you want to compile:

  1. **UNICODE_SUPPORT**: This will allow the output to use unicode characters (don't use it if your shell doesn't support unicode, which most do).
  2. **More in the future...**

**Note**: In Ubuntu 16.04 LTS, the default version for boost is 1.58, which has a bug related to the multiprecision library. Use a later version.

In order to use the macros, just include `#define` directives at the top of your program, like this:

```cpp
#define UNICODE_SUPPORT
#include "polynomials/include/polynomial.hpp"

.....
```

## Extra
There are some handy aliases that can help you to write cleaner code. These are all the aliases provided (under the `fgs` namespace):

```cpp
// Aliases for floating-point coefficients (real numbers)
using polynomial_float = Polynomial<float>;
using polynomial_double = Polynomial<double>;
using polynomial_long_double = Polynomial<long double>;

// Aliases for std::complex coefficients (complex numbers)
using polynomial_complex_float = Polynomial<std::complex<float>>;
using polynomial_complex_double = Polynomial<std::complex<double>>;
using polynomial_complex_long_double = Polynomial<std::complex<long double>>;

// For most porpuses, double coefficients will be preferred, so the canonical
// name will be given to those.
using polynomial = polynomial_double;
```

## Tests
For the moment, the library has two types of test (which will be great if the library pases both of them):

  1. **Validation test**: This test checks that every type/function compiles well **AND** doesn't produce any *unexpected* run-time error.
  2. **Unit tests**: These tests check that the logic behind every operation is correct and results are consistent. For these ones, [Boost.Test](https://www.boost.org/doc/libs/1_67_0/libs/test/doc/html/index.html) is required.

If you decide to collaborate with the development of this library, try to make sure that both tests are passed. If you provide more tests, document them in this *README.md* and make sure they are correct and serve their purpose.

## Format
Even though it's not provided yet, a coding standard will be required when writing the library. There're two fundamental tools that should be used to check the correctness of the code:

  1. **clang-format**: If you decide not to follow the coding format, you should use this tool in order to upload code that follows the standard format (a *.clang-format* file is provided for this purpose).
  2. **clang-tidy**: The code should follow good practices, so this tool will allow you to check if the code follows them (the *Makefile* has a rule to automatically do this check).

Also, it will be great if you use more sanatizers other than **clang-tidy** to find potential bugs or performance issues (cppcheck...).

## Some notes
For the moment, the library only works (more or less) well with floating-point types and their corresponding `std::complex` generalizations. For any other type, behaviour is undefined and some functions can even lead to compile-time errors.

Any suggestions are welcome

##
