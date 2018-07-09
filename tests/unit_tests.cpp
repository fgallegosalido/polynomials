#define UNICODE_SUPPORT
#include "polynomial.hpp"

#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <random>

// Boost's Unit Test Framework
#define BOOST_TEST_MODULE Polynomial Test
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

typedef boost::mpl::list<float, double, long double, std::complex<float>,
                         std::complex<double>, std::complex<long double>> test_types;
[[maybe_unused]] static constexpr std::size_t data = 100, lower_degree = 1, upper_degree = 4;

template <typename T>
inline constexpr auto low_b = fgs::aux::reduce_complex_t<T>(-1.0e+2);
template <typename T>
inline constexpr auto up_b = fgs::aux::reduce_complex_t<T>(1.0e+2);

// Tolerances for the tests to be passed
inline constexpr float tolerance_f = 1.0e-2f;
inline constexpr double tolerance_d = 1.0e-11;
inline constexpr long double tolerance_l = 1.0e-14l;

std::mt19937 gen(std::random_device{}());
template <typename T>
inline static std::uniform_real_distribution<fgs::aux::reduce_complex_t<T>> unif(low_b<T>, up_b<T>);

template <typename InputIterator>
void print_range(InputIterator first, InputIterator last, std::ostream& os = std::cout){
    for (; first != last; ++first)
        os << *first << " ";
    os << std::endl;
}

template <typename T>
bool floating_point_comparison(const T& lhs, const T& rhs){
    using FloatType = fgs::aux::reduce_complex_t<T>;

    if constexpr (std::is_same_v<float, FloatType>)
        return std::abs(lhs - rhs) < tolerance_f;
    else if constexpr (std::is_same_v<double, FloatType>)
        return std::abs(lhs - rhs) < tolerance_d;
    else if constexpr (std::is_same_v<long double, FloatType>)
        return std::abs(lhs - rhs) < tolerance_l;
    else
        return std::abs(lhs - rhs) == 0;
}

template <typename T>
std::enable_if_t<std::is_arithmetic_v<T>> generate_vector(std::vector<T>& v){
    std::generate(v.begin(), v.end(), [&](){
        return unif<T>(gen);
    });
}

template <typename T>
std::enable_if_t<fgs::aux::is_complex_v<T>> generate_vector(std::vector<T>& v){
    std::generate(v.begin(), v.end(), [&](){
        return T(unif<T>(gen), unif<T>(gen));
    });
}

template <typename T, template<typename> typename Op>
bool test_operator(std::size_t n, std::size_t m){
    using TestPol = fgs::Polynomial<T>;

    std::vector<T> v1(n+1), v2(m+1), v;
    generate_vector(v1);
    generate_vector(v2);

    v1.resize(std::max(n, m)+1, T(0));
    v2.resize(std::max(n, m)+1, T(0));

    TestPol p1(v1), p2(v2), p;

    std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(v), Op<T>());
    p = Op<TestPol>()(p1, p2);

    v.resize(p.degree()+1);

    return std::equal(v.begin(), v.end(), p.begin(), p.end(), floating_point_comparison<T>);
}

template <typename T>
bool test_addition(std::size_t n, std::size_t m){
    return test_operator<T, std::plus>(n, m);
}

template <typename T>
bool test_substraction(std::size_t n, std::size_t m){
    return test_operator<T, std::minus>(n, m);
}

template <typename T>
bool test_multiplication(std::size_t n, std::size_t m){
    using TestPol = fgs::Polynomial<T>;

    std::vector<T> v1(n+1), v2(m+1), v(n+m+1, T(0));
    generate_vector(v1);
    generate_vector(v2);

    TestPol p1(v1), p2(v2), p;

    for (std::size_t i=0; i<v1.size(); ++i)
        for (std::size_t j=0; j<v2.size(); ++j)
            v[i+j] += std::multiplies<T>()(v1[i], v2[j]);

    p = std::multiplies<TestPol>()(p1, p2);

    return std::equal(v.begin(), v.end(), p.begin(), p.end(), floating_point_comparison<T>);
}

template <typename T>
bool test_division(std::size_t n, std::size_t m){
    using TestPol = fgs::Polynomial<T>;

    std::vector<T> v1(n+1), v2(m+1), v(std::max(n, m)-std::min(n, m)+1, T(0));
    generate_vector(v1);
    generate_vector(v2);

    TestPol p1(v1.rbegin(), v1.rend()), p2(v2.rbegin(), v2.rend()), p;

    if (n < m)
        v.resize(1);
    else
        for (std::size_t i=0; i<v.size(); ++i){
            v[i] = v1[i] / v2[0];
            for (std::size_t j=0; j<v2.size(); ++j)
                v1[i+j] -= v2[j] * v[i];
        }

    p = std::divides<TestPol>()(p1, p2);

    return std::equal(v.rbegin(), v.rend(), p.begin(), p.end(), floating_point_comparison<T>);
}

template <typename T>
bool test_modulus(std::size_t n, std::size_t m){
    using TestPol = fgs::Polynomial<T>;

    std::vector<T> v1(n+1), v2(m+1), v(std::max(n, m)-std::min(n, m)+1, T(0));
    generate_vector(v1);
    generate_vector(v2);

    TestPol p1(v1.rbegin(), v1.rend()), p2(v2.rbegin(), v2.rend()), p;

    if (n < m)
        v.resize(1);
    else
        for (std::size_t i=0; i<v.size(); ++i){
            v[i] = v1[i] / v2[0];
            for (std::size_t j=0; j<v2.size(); ++j)
                v1[i+j] -= v2[j] * v[i];
        }

    p = std::divides<TestPol>()(p1, p2);

    return std::equal(v.rbegin(), v.rend(), p.begin(), p.end(), floating_point_comparison<T>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(polynomial_test, T, test_types){
    // Random test cases for arithmetic operations
    BOOST_TEST((test_addition<T>(upper_degree, upper_degree*2)));
    BOOST_TEST((test_addition<T>(upper_degree, upper_degree)));
    BOOST_TEST((test_addition<T>(upper_degree*2, upper_degree)));

    BOOST_TEST((test_substraction<T>(upper_degree, upper_degree*2)));
    BOOST_TEST((test_substraction<T>(upper_degree, upper_degree)));
    BOOST_TEST((test_substraction<T>(upper_degree*2, upper_degree)));

    BOOST_TEST((test_multiplication<T>(upper_degree, upper_degree*2)));
    BOOST_TEST((test_multiplication<T>(upper_degree, upper_degree)));
    BOOST_TEST((test_multiplication<T>(upper_degree*2, upper_degree)));

    BOOST_TEST((test_division<T>(upper_degree, upper_degree*2)));
    BOOST_TEST((test_division<T>(upper_degree, upper_degree)));
    BOOST_TEST((test_division<T>(upper_degree*2, upper_degree)));

    // Edge cases for arithmetic operations
    BOOST_TEST((test_addition<T>(upper_degree, 0)));
    BOOST_TEST((test_addition<T>(0, 0)));
    BOOST_TEST((test_addition<T>(0, upper_degree)));

    BOOST_TEST((test_substraction<T>(upper_degree, 0)));
    BOOST_TEST((test_substraction<T>(0, 0)));
    BOOST_TEST((test_substraction<T>(0, upper_degree)));

    BOOST_TEST((test_multiplication<T>(upper_degree, 0)));
    BOOST_TEST((test_multiplication<T>(0, 0)));
    BOOST_TEST((test_multiplication<T>(0, upper_degree)));

    BOOST_TEST((test_division<T>(upper_degree, 0)));
    BOOST_TEST((test_division<T>(0, 0)));
    BOOST_TEST((test_division<T>(0, upper_degree)));
}
