#include <iostream>
#include <vector>
#include <functional>
#include <complex>
#include <cmath>
#include <limits>
#include <random>

#define UNICODE_SUPPORT
#include "polynomial.hpp"
#include "polynomial_io.hpp"

template <typename InputIterator>
void print_range(InputIterator first, InputIterator last, std::ostream& os = std::clog){
    for (; first != last; ++first)
        os << *first << " ";
    os << "\n";
}

int main() {
    using namespace std::complex_literals;
    std::clog.precision(3);

    fgs::polynomial_complex_double c = {{0,1}, 1, {0,-1}};

    std::vector<double> v = {2.1, 2, 1};
    fgs::polynomial_double p1(v.begin(), v.end());
    fgs::polynomial_double p2 = {1, 1};
    auto p3 = p1+p2;
    auto p4 = p1-p2;
    auto p5 = p1*p2;
    auto p6 = p1/p2;
    auto p7 = p1%p2;

    std::clog << "p1: " << p1 << "\n";
    std::clog << "-----------\n";
    std::clog << "p2: " << p2 << "\n";
    std::clog << "-----------\n";
    std::clog << "Addition: " << p3 << "\n";
    std::clog << "-----------\n";
    std::clog << "Substraction: " << p4 << "\n";
    std::clog << "-----------\n";
    std::clog << "Product: " << p5 << "\n";
    std::clog << "-----------\n";
    std::clog << "Division: " << p6 << "\n";
    std::clog << "-----------\n";
    std::clog << "Reminder: " << p7 << "\n";
    std::clog << "-----------\n";
    std::clog << "Evaluation: " << p1(2) << "\n";
    std::clog << "-----------\n";
    std::clog << "GCD: " << fgs::gcd(p1, p2) << "\n";
    std::clog << "-----------\n";
    std::clog << "LCM: " << fgs::lcm(p1, p2) << "\n";
    std::clog << "-----------\n";
    std::clog << "Differentiate: " << fgs::differentiate(p5) << "\n";
    std::clog << "-----------\n";
    std::clog << "Pow: " << fgs::pow(fgs::polynomial_double{-1, 1}, 2) << "\n";
    std::clog << "-----------\n";
    std::clog << std::boolalpha << "Equal comparison: " << (p5 == lcm(p1,p2)) << "\n";
    std::clog << "-----------\n";

    std::vector<std::vector<std::complex<long double>>> w(7);

    w[0] = fgs::roots_of(fgs::make_polynomial_by_roots({2.5}));
    w[1] = fgs::roots_of(fgs::make_polynomial_by_roots({2.5, -0.33}));
    w[2] = fgs::roots_of(fgs::make_polynomial_by_roots({2.5, -0.33, 7.4}));
    w[3] = fgs::roots_of(fgs::make_polynomial_by_roots({2.5, -0.33, 7.4, -5.0}));
    w[4] = fgs::roots_of(fgs::make_polynomial_by_roots({2.5, -0.33, 7.4, -5.0, 3.97}));
    w[5] = fgs::roots_of(fgs::polynomial{1, 0, 1}*fgs::polynomial{-1, 1});
    w[6] = fgs::roots_of(fgs::make_polynomial_by_roots({5.0, 5.0})*fgs::polynomial{1, 0, 1});

    for (const auto& i : w){
        for (const auto& j : i)
            std::clog << j << " ";
        std::clog << "\n";
    }
    std::clog << "\n";

    {
        std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<> unif{0.0, 1.0e10};
        std::size_t n_tests = 100;

        std::vector<std::complex<double>> test_v(n_tests);
        std::generate(test_v.begin(), test_v.end(), [&](){
            return std::complex<double>{unif(gen), unif(gen)};
        });

        for (std::size_t i=1; i<=n_tests; ++i){
            std::clog << "Degree " << i << "\n";
            std::vector<std::complex<double>> sample;
            std::sample(test_v.begin(), test_v.end(), std::back_inserter(sample), i, gen);

            auto result = fgs::roots_of(fgs::polynomial_complex{sample});
        }
    }

    std::clog << fgs::taylor_polynomial(0.0, {1}) << "\n";
    std::clog << fgs::taylor_polynomial(0.0, {1, 1}) << "\n";
    std::clog << fgs::taylor_polynomial(0.0, {1, 1, 1}) << "\n";
    std::clog << fgs::taylor_polynomial(0.0, {1, 1, 1, 1}) << "\n";
    std::clog << fgs::taylor_polynomial(0.0, {1, 1, 1, 1, 1}) << "\n";

    auto exp_aprox = fgs::taylor_polynomial(0.0, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
    std::clog << exp_aprox(1) << "\n";

    for (unsigned i=1; i<=7; ++i)
        std::clog << fgs::cyclotomic_polynomial<double>(i) << "\n";

    std::clog << fgs::reduced_polynomial(fgs::polynomial{4, 4, 2.0}) << "\n";
}
