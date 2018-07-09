#define UNICODE_SUPPORT
#include "polynomial.hpp"

#include <iostream>
#include <vector>
#include <functional>
#include <complex>
#include <cmath>
#include <limits>

template <typename InputIterator>
void print_range(InputIterator first, InputIterator last, std::ostream& os = std::cout){
    for (; first != last; ++first)
        os << *first << " ";
    os << std::endl;
}

int main() {
    using namespace std::complex_literals;
    fgs::polynomial_complex_double c = {{0,1}, 1, {0,-1}};

    std::vector<double> v = {2.1, 2, 1};
    fgs::polynomial_double p1(v.begin(), v.end());
    fgs::polynomial_double p2 = {1, 1};
    auto p3 = p1+p2;
    auto p4 = p1-p2;
    auto p5 = p1*p2;
    auto p6 = p1/p2;
    auto p7 = p1%p2;

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
    std::cout << "GCD: " << fgs::gcd(p1, p2) << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << "LCM: " << fgs::lcm(p1, p2) << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << "Differentiate: " << fgs::differentiate(p5) << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << "Pow: " << fgs::pow(fgs::polynomial_double{-1, 1}, 2) << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << std::boolalpha << "Equal comparison: " << (p5 == lcm(p1,p2)) << std::endl;
    std::cout << "-----------" << std::endl;

    std::vector<std::vector<std::complex<double>>> w(5);

    w[0] = fgs::roots_of(fgs::make_polynomial_by_roots({2.5}));
    w[1] = fgs::roots_of(fgs::make_polynomial_by_roots({2.5, -0.33}));
    w[2] = fgs::roots_of(fgs::make_polynomial_by_roots({2.5, -0.33, 7.4}));
    w[3] = fgs::roots_of(fgs::make_polynomial_by_roots({2.5, -0.33, 7.4, -5.0}));
    w[4] = fgs::roots_of(fgs::make_polynomial_by_roots({2.5, -0.33, 7.4, -5.0, 3.97}));

    for (const auto& i : w){
        for (const auto& j : i)
            std::cout << j << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << fgs::taylor_polynomial(0.0, {1}) << std::endl;
    std::cout << fgs::taylor_polynomial(0.0, {1, 1}) << std::endl;
    std::cout << fgs::taylor_polynomial(0.0, {1, 1, 1}) << std::endl;
    std::cout << fgs::taylor_polynomial(0.0, {1, 1, 1, 1}) << std::endl;
    std::cout << fgs::taylor_polynomial(0.0, {1, 1, 1, 1, 1}) << std::endl;

    auto exp_aprox = fgs::taylor_polynomial(0.0, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
    std::cout << exp_aprox(1) << std::endl;
    std::cout << 
}
