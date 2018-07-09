#define UNICODE_SUPPORT
#include "polynomial.hpp"

#include <iostream>
#include <sstream>
#include <vector>

// clang-format off
#ifdef __clang__
    #pragma clang diagnostic ignored "-Wunused-comparison"
#endif

template <typename T>
void test_runtime_validation_specific() {
    using TestPol = fgs::Polynomial<T>;
    using RootType = std::conditional_t<fgs::aux::is_complex_v<T>, T, std::complex<T>>;

    TestPol p0, p1(1), p2{1, 2, 3, 4};  // Direct-value constructors
    TestPol p3(p2), p4(std::move(p3)), p5 = {1, 2, 3, 4}, p6 = p5;  // Copy/Move constructors
    std::vector<T> v{1, 2, 3, 4};
    TestPol p7(v.begin(), v.end()), p8(v);  // Range/Container constructors

    p0 = p2;  // Copy assignment
    p4 = std::move(p5);  // Move assignment

    // Polynomial construction from the roots
    TestPol p9 = fgs::make_polynomial_by_roots(v.begin(), v.end());
    TestPol p10 = fgs::make_polynomial_by_roots(v);
    TestPol p11 = fgs::make_polynomial_by_roots({1, 2, 3, 4});

    TestPol q0{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, q1{0, 2, 4};
    T x(10);

    // Getters and setters
    q0[0]; q0[0] = 1;
    q0.get_coefficient(0); q0.get_coefficient(0) = 1;
    q0.first(); q0.first() = 1;
    q0.last(); q0.last() = 10;
    q0.set_coefficient(9, 10);
    q0.set_coefficient(10, 11);
    q0.get_variable();
    q0.set_variable('y');

    q0.degree();  // Degree function

    // Evaluation
    q0.evaluate_at(10);
    q0(10);

    // Unary arithmetic operators
    +q0; -q0;

    // Binary arithmetic operators
    q0 + q1; q0 + x; x + q0;
    q0 - q1; q0 - x; x - q0;
    q0 * q1; q0 * x; x * q0;
    q0 / q1; q0 / x; x / q0;
    q0 % q1; q0 % x; x % q0;

    // Atomic binary arithmetic operators
    q0 += q1; q0 += x;
    q0 -= q1; q0 -= x;
    q0 *= q1; q0 *= x;
    q0 /= q1; q0 /= x;
    q0 %= q1; q0 %= x;

    TestPol q2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, q3{0, 2, 4};

    // Comparison operators
    q2 == q3; q2 != q3;

    // Special operations
    q2.pow(2);                  fgs::pow(q2, 2);
    q2.differentiate();         fgs::differentiate(q2);
    q2.integrate_const();       fgs::integrate_const(q2);
    q2.integrate_const(x);      fgs::integrate_const(q2, x);
    q2.integrate_point(x, x);   fgs::integrate_point(q2, x, x);

    fgs::definite_integral(q2, -1, 1);
    fgs::gcd(q2, q3);
    fgs::lcm(q2, q3);

    // Swap function
    std::swap(q2, q3);

    TestPol r0{1}, r1{1, 2}, r2{1, 2, 3}, r3{1, 2, 3, 4}, r4{1, 2, 3, 4, 5}, r5{1, 2, 3, 4, 5, 6};
    std::vector<RootType> w;

    // Root-finding function with back_inserter iterator
    fgs::roots_of(r0, std::back_inserter(w)); w.clear();
    fgs::roots_of(r1, std::back_inserter(w)); w.clear();
    fgs::roots_of(r2, std::back_inserter(w)); w.clear();
    fgs::roots_of(r3, std::back_inserter(w)); w.clear();
    fgs::roots_of(r4, std::back_inserter(w)); w.clear();
    fgs::roots_of(r5, std::back_inserter(w)); w.clear();

    // Root-finding function returning a container
    w = fgs::roots_of(r5);

    std::vector<T> l1{1, 2, 3, 4}, l2{1, 4, 9, 16};
    auto f = [](T x) -> T {return x*x;};

    // Lagrange polynomial
    fgs::lagrange_polynomial(l1.begin(), l1.end(), l2.begin(), l2.end());
    fgs::lagrange_polynomial(l1, l2);
    fgs::lagrange_polynomial({T(1), T(2), T(3), T(4)}, {T(1), T(4), T(9), T(16)});
    fgs::lagrange_polynomial(l1, {T(1), T(4), T(9), T(16)});
    fgs::lagrange_polynomial({T(1), T(2), T(3), T(4)}, l2);
    fgs::lagrange_polynomial(l1.begin(), l1.end(), f);
    fgs::lagrange_polynomial(l1, f);
    fgs::lagrange_polynomial({T(1), T(2), T(3), T(4)}, f);

    std::vector<T> t{1, 1, 1, 1};

    fgs::taylor_polynomial(0, t.begin(), t.end());
    fgs::taylor_polynomial(0, t);
    fgs::taylor_polynomial(0, {T(1), T(1), T(1), T(1)});

    // Output test;
    TestPol s{12, 9, 6, 3, 1};
    std::ostringstream os; os << s;
}

template <typename T, typename U>
void test_runtime_validation_specific_binary (){
    using TestPol1 = fgs::Polynomial<T>;
    using TestPol2 = fgs::Polynomial<U>;

    TestPol2 aux1{1, 2, 3, 4}, aux2{1, 2, 3, 4};

    TestPol1 p0, p1(U(1));  // Direct-value constructors
    TestPol1 p3(aux1), p4(std::move(aux1)), p6 = aux2;  // Copy/Move constructors
    std::vector<U> v{1, 2, 3, 4};
    TestPol1 p7(v.begin(), v.end()), p8(v);  // Range/Container constructors

    p0 = aux2;  // Copy assignment
    p4 = std::move(aux2);  // Move assignment

    TestPol1 q1{1, 2, 3};
    TestPol2 q2{1, 2, 3};
    U y(1);

    // Binary arithmetic operators
    q1 + q2; q2 + q1; q1 + y; y + q1;
    q1 - q2; q2 - q1; q1 - y; y - q1;
    q1 * q2; q2 * q1; q1 * y; y * q1;
    q1 / q2; q2 / q1; q1 / y; y / q1;
    q1 % q2; q2 % q1; q1 % y; y % q1;

    // Atomic binary arithmetic operators
    q1 += q2; q1 += y;
    q1 -= q2; q1 -= y;
    q1 *= q2; q1 *= y;
    q1 /= q2; q1 /= y;
    q1 %= q2; q1 %= y;

    // Comparison operators
    q1 == q2; q2 == q1;
    q1 != q2; q2 != q1;
}

void test_runtime_validation() {
    // Validation for floating-point native types
    test_runtime_validation_specific<float>();
    test_runtime_validation_specific<double>();
    test_runtime_validation_specific<long double>();

    // Validation for complex types
    test_runtime_validation_specific<std::complex<float>>();
    test_runtime_validation_specific<std::complex<double>>();
    test_runtime_validation_specific<std::complex<long double>>();

    // Validation between floating-point native types
    test_runtime_validation_specific_binary<float, double>();
    test_runtime_validation_specific_binary<float, long double>();
    test_runtime_validation_specific_binary<double, float>();
    test_runtime_validation_specific_binary<double, long double>();
    test_runtime_validation_specific_binary<long double, float>();
    test_runtime_validation_specific_binary<long double, double>();

    // Validation between complex types
    test_runtime_validation_specific_binary<std::complex<float>, std::complex<double>>();
    test_runtime_validation_specific_binary<std::complex<float>, std::complex<long double>>();
    test_runtime_validation_specific_binary<std::complex<double>, std::complex<float>>();
    test_runtime_validation_specific_binary<std::complex<double>, std::complex<long double>>();
    test_runtime_validation_specific_binary<std::complex<long double>, std::complex<float>>();
    test_runtime_validation_specific_binary<std::complex<long double>, std::complex<double>>();

    // Validation between floating-point native types and complex types
    test_runtime_validation_specific_binary<std::complex<float>, float>();
    test_runtime_validation_specific_binary<std::complex<float>, double>();
    test_runtime_validation_specific_binary<std::complex<float>, long double>();
    test_runtime_validation_specific_binary<std::complex<double>, float>();
    test_runtime_validation_specific_binary<std::complex<double>, double>();
    test_runtime_validation_specific_binary<std::complex<double>, long double>();
    test_runtime_validation_specific_binary<std::complex<long double>, float>();
    test_runtime_validation_specific_binary<std::complex<long double>, double>();
    test_runtime_validation_specific_binary<std::complex<long double>, long double>();
}

int main() {
    test_runtime_validation();
}
// clang-format on
