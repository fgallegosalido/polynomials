#pragma once

#include "detail.hpp"

#include <vector>
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <utility>
#include <complex>
#include <numeric>
#include <exception>

namespace fgs{

// Alias to get the category of an iterator (random access, bidirectional,...)
template <typename Iterator>
using traits_category = typename std::iterator_traits<Iterator>::iterator_category;
// Alias to get the value type pointed by an iterator
template <typename Iterator>
using traits_type = typename std::iterator_traits<Iterator>::value_type;

// Class Polynomial. CType is the type of the coefficients.
//
// CType should behave as a field (float, double, long double and the
// std::complex counter-parts meet this requirement)
template <typename CType>
class Polynomial{
public:

    // We make every template specialization a friend class
    template <typename CType2>
    friend class Polynomial;

    // typedefs for the member types (just get the ones from the
    // underlined container).
    using container_type            = std::vector<CType>;
    using value_type                = typename container_type::value_type;
    using allocator_type            = typename container_type::allocator_type;
    using size_type                 = typename container_type::size_type;
    using difference_type           = typename container_type::difference_type;
    using reference                 = typename container_type::reference;
    using const_reference           = typename container_type::const_reference;
    using pointer                   = typename container_type::pointer;
    using const_pointer             = typename container_type::const_pointer;
    using iterator                  = typename container_type::iterator;
    using const_iterator            = typename container_type::const_iterator;
    using reverse_iterator          = typename container_type::reverse_iterator;
    using const_reverse_iterator    = typename container_type::const_reverse_iterator;

    // Default constructors, operator='s and destructor
    Polynomial (const Polynomial&)                                       = default;
    Polynomial (Polynomial&&)
        noexcept(noexcept(container_type(container_type())))             = default;
    Polynomial& operator= (const Polynomial&)                            = default;
    Polynomial& operator= (Polynomial&&)
        noexcept(noexcept(container_type().operator=(container_type()))) = default;
    ~Polynomial ()                                                       = default;


    // Some constructors
    explicit Polynomial ()
        : coeffs(1, value_type(0)) {}
    template <typename U>
    explicit Polynomial (const U& x)
        : coeffs(1, value_type(x)) {}
    Polynomial (std::initializer_list<value_type> l)
        : coeffs(l) { adjust_degree(); }

    // Range constructor using tag dispatching for input iterators
    template <typename InputIt>
    Polynomial (InputIt first, InputIt last)
        : coeffs(first, last) { adjust_degree(); }

    // Constructor using the elements of a container (in the future
    // also ranges will be accepted)
    template <template<typename...> typename Cont, typename ...Args,
                typename = std::enable_if_t<detail::is_iterable_v<Cont<Args...>>>>
    explicit Polynomial (const Cont<Args...>& cont)
        : Polynomial(std::begin(cont), std::end(cont)) {}

    reference       operator[] (size_type i)       { return coeffs[i]; }
    const_reference operator[] (size_type i) const { return coeffs[i]; }

    reference       at (size_type i)       { return coeffs.at(i); }
    const_reference at (size_type i) const { return coeffs.at(i); }

    reference       first()       { return coeffs.front(); }
    const_reference first() const { return coeffs.front(); }

    reference       last()       { return coeffs.back(); }
    const_reference last() const { return coeffs.back(); }

    void set_coefficient (size_type i, const value_type& elem){
        if (i >= coeffs.size())
            coeffs.resize(i+1, value_type(0));

        coeffs[i] = elem;
        adjust_degree();
    }

    char get_variable () const noexcept{ return var; }
    void set_variable (char c) noexcept{ var = c; }

    size_type degree () const noexcept{ return coeffs.size()-1; }

    /* Evaluates the polynomial for the value x using the Horner's
     * polynomial evaluation scheme.
     *
     * RType is the type of the evaluation
     */
    template<typename RType>
    auto evaluate_at (const RType& x) const{
        using CommonType = std::common_type_t<value_type, RType>;
        CommonType res(coeffs.back());

        for (size_type i=coeffs.size()-1; i>0; --i)
            res = CommonType(coeffs[i-1]) + res*CommonType(x);

        return res;
    }

    template <typename CType2>
    auto evaluate_at (const Polynomial<CType2> &pol) const{
        using CommonType = std::common_type_t<value_type, CType2>;
        Polynomial<CommonType> res(coeffs.back());

        for (size_type i=coeffs.size()-1; i>0; --i)
            res = CommonType(coeffs[i-1]) + res*Polynomial<CommonType>(pol);

        return res;
    }

    /* Enable natural evaluation of a mathematical function, so you
     * can write p(x) instead of p.evaluate_at(x). RType is the type
     * of the evaluation
     */
    template<typename RType>
    auto operator() (const RType& x) const{
        return evaluate_at(x);
    }

    // Unary + and - operators
    Polynomial operator+() const { return Polynomial(*this); }
    Polynomial operator-() const { return Polynomial(*this) *= value_type(-1); }

    // This is just a cool way to
    Polynomial operator^ (unsigned n){
        return Polynomial(*this).pow(n);
    }

    // Operator overloadings for polynomials arithmetic
    template <typename T>
    Polynomial& operator+= (const Polynomial<T>& pol){
        coeffs.resize(std::max(coeffs.size(), pol.coeffs.size()), value_type(0));

        for (size_type i=0; i<pol.coeffs.size(); ++i)
            coeffs[i] += value_type(pol.coeffs[i]);

        adjust_degree();
        return *this;
    }

    template <typename T>
    Polynomial& operator-= (const Polynomial<T>& pol){
        coeffs.resize(std::max(coeffs.size(), pol.coeffs.size()), value_type(0));

        for (size_type i=0; i<pol.coeffs.size(); ++i)
            coeffs[i] -= value_type(pol.coeffs[i]);

        adjust_degree();
        return *this;
    }

    template <typename T>
    Polynomial& operator*= (const Polynomial<T>& pol){
        coeffs.resize(coeffs.size()+pol.coeffs.size()-1, value_type(0));

        for (int i=coeffs.size()-pol.coeffs.size(); i>=0; --i){
            for (size_type j=pol.coeffs.size()-1; j>0; --j)
                coeffs[i+j] += coeffs[i]*value_type(pol.coeffs[j]);
            coeffs[i] = coeffs[i]*value_type(pol.coeffs[0]);
        }

        return *this;
    }

    template <typename T>
    Polynomial& operator/= (const Polynomial<T>& pol){
        if (coeffs.size() < pol.coeffs.size()){
            coeffs.resize(1);
            coeffs[0] = value_type(0);
            return *this;
        }

        container_type coc(coeffs.size()-pol.coeffs.size()+1, value_type(0));

        for (int i=coc.size()-1; i>=0; --i){
            coc[i] = coeffs[pol.coeffs.size()+i-1]/value_type(pol.coeffs.back());
            for (int j=pol.coeffs.size()-2; j>=0; --j)
                coeffs[i+j] -= value_type(pol.coeffs[j])*coc[i];
        }

        coeffs = std::move(coc);
        return *this;
    }

    template <typename T>
    Polynomial& operator%= (const Polynomial<T>& pol){
        if (coeffs.size() < pol.coeffs.size())
            return *this;

        value_type coc;

        for (int i=coeffs.size()-pol.coeffs.size(); i>=0; --i){
            coc = coeffs[pol.coeffs.size()+i-1]/value_type(pol.coeffs.back());
            for (int j=pol.coeffs.size()-1; j>=0; --j)
                coeffs[i+j] -= value_type(pol.coeffs[j])*coc;
        }

        adjust_degree();
        return *this;
    }

    template <typename U>
    Polynomial& operator+=(const U& other){
        coeffs[0] += value_type(other);
        return *this;
    }
    template <typename U>
    Polynomial& operator-=(const U& other){
        coeffs[0] -= value_type(other);
        return *this;
    }
    template <typename U>
    Polynomial& operator*=(const U& other){
        for (auto &coeff : coeffs)
            coeff *= value_type(other);

        return *this;
    }
    template <typename U>
    Polynomial& operator/=(const U& other){
        for (auto &coeff : coeffs)
            coeff /= value_type(other);

        return *this;
    }
    template <typename U>
    Polynomial& operator%=(const U&){   // NOLINT(readability-named-parameter)
        coeffs.resize(1, 0);
        return *this;
    }

    /* Modifies the coefficients so they match the polynomial to the
     * power of n (n must be unsigned type)
     */
    Polynomial& pow(unsigned n) {
        if (n==0){
            coeffs.resize(1);
            coeffs.front() = value_type(1);
            return *this;
        }

        Polynomial aux(*this);

        for (unsigned i=1; i<n; ++i)
            (*this) *= aux;

        return *this;
    }

    /* Modifies the coefficients so they match the derivative of
     * the polynomial defined by *this
     */
    Polynomial& differentiate (){
        if (coeffs.size() == 1)
            coeffs[0] = value_type(0);
        else{
            for (size_type i=1; i<coeffs.size(); ++i)
                coeffs[i-1] = coeffs[i]*value_type(i);
            coeffs.resize(coeffs.size()-1);
        }

        return *this;
    }

    /* Modifies the coefficients so they match the antiderivative, with
     * integration constant equals to c (0 by default)
     */
    Polynomial& integrate_const (const value_type& c = value_type(0)){
        if (coeffs.size()==1 && coeffs[0]==value_type(0)){
            coeffs[0] = c;
            return *this;
        }

        coeffs.resize(coeffs.size()+1);

        for (size_type i=coeffs.size()-1; i>0; --i)
            coeffs[i] = coeffs[i-1]/value_type(i);
        coeffs[0] = c;

        return *this;
    }

    /* Modifies the coefficients so they match the antiderivative that
     * meets the condition of F(x)=y (where F is the antiderivative).
     * RType is the type of the evaluation
     */
    template<typename DType, typename RType>
    Polynomial& integrate_point (const DType& x, const RType& y){
        using CommonType = std::common_type_t<CType, DType, RType>;
        coeffs[0] = value_type(CommonType(y) - CommonType((*this).integrate_const().evaluate_at(x)));
        return *this;
    }

    // Swap two polynomials
    friend void swap (Polynomial& lhs, Polynomial& rhs){
        using std::swap;

        swap(lhs.coeffs, rhs.coeffs);
        swap(lhs.var, rhs.var);
    }

    // Conversion from a Polynomial to other of different value_type
    template <typename CType2>
    explicit operator Polynomial<CType2> () const{
        return Polynomial<CType2>(this->begin(), this->end());
    }

    /* Iterator functions to iterate through a polynomial
     *
     * It's based on the vector's iterator, so this is just a wrapper
     * for polynomials
     */
    iterator        begin()       { return coeffs.begin(); }
    const_iterator  begin() const { return coeffs.begin(); }

    iterator        end()       { return coeffs.end(); }
    const_iterator  end() const { return coeffs.end(); }

    reverse_iterator        rbegin()       { return coeffs.rbegin(); }
    const_reverse_iterator  rbegin() const { return coeffs.rbegin(); }

    reverse_iterator        rend()       { return coeffs.rend(); }
    const_reverse_iterator  rend() const { return coeffs.rend(); }

    const_iterator cbegin() const { return coeffs.cbegin(); }
    const_iterator cend()   const { return coeffs.cend();   }

    const_reverse_iterator crbegin() const { return coeffs.crbegin(); }
    const_reverse_iterator crend()   const { return coeffs.crend();   }

private:
    container_type coeffs;  // Actual coefficients of the polynomial
    char var = 'x';         // Letter that identifies the variable

    // Helper function to adjust the degree, so the last coefficient is not 0
    void adjust_degree (){
        while (coeffs.back() == value_type(0) && coeffs.size() > 1)
            coeffs.pop_back();
    }
};

template <typename CType1, typename CType2>
auto operator+(const Polynomial<CType1>& lhs, const Polynomial<CType2>& rhs){
    return Polynomial<std::common_type_t<CType1, CType2>>(lhs) += rhs;
}
template <typename CType1, typename CType2>
auto operator-(const Polynomial<CType1>& lhs, const Polynomial<CType2>& rhs){
    return Polynomial<std::common_type_t<CType1, CType2>>(lhs) -= rhs;
}
template <typename CType1, typename CType2>
auto operator*(const Polynomial<CType1>& lhs, const Polynomial<CType2>& rhs){
    return Polynomial<std::common_type_t<CType1, CType2>>(lhs) *= rhs;
}
template <typename CType1, typename CType2>
auto operator/(const Polynomial<CType1>& lhs, const Polynomial<CType2>& rhs){
    return Polynomial<std::common_type_t<CType1, CType2>>(lhs) /= rhs;
}
template <typename CType1, typename CType2>
auto operator%(const Polynomial<CType1>& lhs, const Polynomial<CType2>& rhs){
    return Polynomial<std::common_type_t<CType1, CType2>>(lhs) %= rhs;
}

template <typename CType, typename U>
auto operator+(const Polynomial<CType>& lhs, const U& rhs){
    return Polynomial<std::common_type_t<CType, U>>(lhs) += rhs;
}
template <typename CType, typename U>
auto operator-(const Polynomial<CType>& lhs, const U& rhs){
    return Polynomial<std::common_type_t<CType, U>>(lhs) -= rhs;
}
template <typename CType, typename U>
auto operator*(const Polynomial<CType>& lhs, const U& rhs){
    return Polynomial<std::common_type_t<CType, U>>(lhs) *= rhs;
}
template <typename CType, typename U>
auto operator/(const Polynomial<CType>& lhs, const U& rhs){
    return Polynomial<std::common_type_t<CType, U>>(lhs) /= rhs;
}
template <typename CType, typename U>
auto operator%(const Polynomial<CType>& lhs, const U& rhs){
    return Polynomial<std::common_type_t<CType, U>>(lhs) %= rhs;
}

template <typename CType, typename U>
auto operator+(const U& lhs, const Polynomial<CType>& rhs){
    return Polynomial<std::common_type_t<CType, U>>(lhs) += rhs;
}
template <typename CType, typename U>
auto operator-(const U& lhs, const Polynomial<CType>& rhs){
    return Polynomial<std::common_type_t<CType, U>>(lhs) -= rhs;
}
template <typename CType, typename U>
auto operator*(const U& lhs, const Polynomial<CType>& rhs){
    return Polynomial<std::common_type_t<CType, U>>(lhs) *= rhs;
}
template <typename CType, typename U>
auto operator/(const U& lhs, const Polynomial<CType>& rhs){
    return Polynomial<std::common_type_t<CType, U>>(lhs) /= rhs;
}
template <typename CType, typename U>
auto operator%(const U& lhs, const Polynomial<CType>& rhs){
    return Polynomial<std::common_type_t<CType, U>>(lhs) %= rhs;
}

template <typename CType1, typename CType2>
bool operator==(const Polynomial<CType1>& lhs, const Polynomial<CType2>& rhs){
    using CommonType = std::common_type_t<CType1, CType2>;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
        [](const CommonType& a, const CommonType& b){
            return a == b;
        }
    );
}

template <typename CType1, typename CType2>
bool operator!=(const Polynomial<CType1>& lhs, const Polynomial<CType2>& rhs){
    return !(lhs == rhs);
}

/* Like Polynomial::pow, but returning an rvalue
 * (not modifying the original)
 */
template <typename CType>
Polynomial<CType> pow(const Polynomial<CType>& pol, unsigned n) {
    return Polynomial<CType>(pol).pow(n);
}

/* Like Polynomial::differentiate, but returning an rvalue
 * (not modifying the original)
 */
template <typename CType>
Polynomial<CType> differentiate (const Polynomial<CType>& pol){
    return Polynomial<CType>(pol).differentiate();
}

/* Like Polynomial::integrate_const, but returning an rvalue
 * (not modifying the original)
 */
template <typename CType>
Polynomial<CType> integrate_const (const Polynomial<CType>& pol, const CType& c = CType(0)){
    return Polynomial<CType>(pol).integrate_const(c);
}

/* Like Polynomial::integrate_point, but returning an rvalue
 * (not modifying the original)
 */
template <typename CType, typename RType>
Polynomial<CType> integrate_point (const Polynomial<CType>& pol, const RType& x, const RType& y){
    return Polynomial<CType>(pol).integrate_point(x, y);
}

/* Calculates the definite integral between two values of the
 * polynomial. RType is the type of the evaluation
 */
template <typename CType, typename RType>
auto definite_integral (const Polynomial<CType>& pol, const RType& lower_bound, const RType& upper_bound){
    Polynomial<CType> p(pol);
    p.integrate_const();
    return p.evaluate_at(upper_bound) - p.evaluate_at(lower_bound);
}

// GCD of two polynomials using Euclidean's algorithm
template <typename CType>
Polynomial<CType> gcd (Polynomial<CType> lhs, Polynomial<CType> rhs){
    if (lhs==Polynomial<CType>())
        return rhs;
    if (rhs==Polynomial<CType>())
        return lhs;

    while (rhs.degree()>0 || rhs[0]!=CType(0)){
        swap(lhs, rhs);
        rhs %= lhs;
    }

    return lhs;
}

// LCM of two polynomials using Euclidean's algorithm
template <typename CType>
const Polynomial<CType> lcm (const Polynomial<CType>& lhs, const Polynomial<CType>& rhs){
    return (lhs/gcd(lhs, rhs))*rhs;
}

namespace roots_aux{
    inline static constexpr long double TOLERANCE = 1e-11L;
    inline static constexpr long double TWO_PI = 6.2831853071795864769L;

    inline long double lower_bound_roots (const Polynomial<std::complex<long double>> &p){
        return std::abs(p[0]) / std::min(
            std::abs(p.first()) + std::abs(*std::max_element(std::next(p.begin()), p.end(),
                [](const auto &lhs, const auto &rhs){
                    return std::abs(lhs) < std::abs(rhs);
                })),
            std::max(std::abs(p.first()), std::accumulate(std::next(p.begin()), std::prev(p.end()), 1.0L,
                [](const auto &acc, const auto &rhs){
                    return acc + std::abs(rhs);
                }))
        );
    }

    inline long double upper_bound_roots (const Polynomial<std::complex<long double>> &p){
        return std::min(
            1.0L + std::abs(*std::max_element(p.begin(), std::prev(p.end()),
                [](const auto &lhs, const auto &rhs){
                    return std::abs(lhs) < std::abs(rhs);
                })),
            std::max(1.0L, std::accumulate(p.begin(), std::prev(p.end()), 0.0L,
                [](const auto &acc, const auto &rhs){
                    return acc + std::abs(rhs);
                }))
        );
    }

    inline auto generate_values (const Polynomial<std::complex<long double>> &p){
        using namespace std::complex_literals;

        long double
            low = lower_bound_roots(p),
            high = upper_bound_roots(p),
            mod = (low + high) / 2.0L;

        if (mod == 1.0L)
            mod -= (high - low) / 4.0L;

        auto rot = std::exp(1.0il * (TWO_PI / static_cast<long double>(p.degree())));
        std::vector<std::complex<long double>> ret{mod*std::pow(rot, 0.25L)};
        ret.reserve(p.degree());

        for (std::size_t i=1; i<p.degree(); ++i)
            ret.push_back(ret[i-1] * rot);

        return ret;
    }

    template <typename OutputIterator>
    void durand_kerner(const Polynomial<std::complex<long double>> &p, OutputIterator out){
        std::vector<std::pair<std::complex<long double>, std::complex<long double>>> ret;
        ret.reserve(p.degree());

        for (auto && elem : generate_values(p))
            ret.emplace_back(std::complex<long double>{}, elem);

        while (!std::all_of(ret.begin(), ret.end(),
            [](const auto &e){
                return std::abs(e.second - e.first) < TOLERANCE;
            }))
        {
            for (std::size_t i=0; i<ret.size(); ++i){
                ret[i].first = ret[i].second;
                std::complex<long double> aux{1.0L};

                for (std::size_t j=0; j<i; ++j)
                    aux *= (ret[i].second - ret[j].second);
                for (std::size_t j=i+1; j<ret.size(); ++j)
                    aux *= (ret[i].second - ret[j].second);

                ret[i].second -= p(ret[i].second) / aux;
            }
        }

        *out = ret.front().second;
        for (auto it = std::next(ret.begin()); it != ret.end(); ++it)
            *++out = it->second;
        }

    template <typename OutputIterator>
    void roots_of_impl (const Polynomial<std::complex<long double>> &p, OutputIterator out){
        // For polynomials of degree less than 3, we will just use the formulas
        if (p.degree() == 1)
            *out = -p[0];
        else if (p.degree() == 2){
            std::complex<long double> discriminant(std::sqrt(p[1]*p[1] - 4.0L*p[0]));
            *out = (-p[1] + discriminant) / 2.0L;
            *++out = (-p[1] - discriminant) / 2.0L;
        }
        else if (p.degree() >= 3)
            // For polynomials of degree higher than 3, we will use
            // Durand-Kerner method, as stability for higher degree formulas
            // is really bad
            durand_kerner(p, out);
    }
}

template <typename CType, typename OutputIterator>
void roots_of(const Polynomial<CType>& p, OutputIterator out){
    auto aux =  static_cast<Polynomial<std::complex<long double>>>(p) /
                static_cast<std::complex<long double>>(p.last());

    roots_aux::roots_of_impl(aux, out);
}

template <template <typename...> typename Cont = std::vector, typename CType>
auto roots_of (const Polynomial<CType>& pol) {
    Cont<std::complex<long double>> roots;
    roots_of(pol, std::back_inserter(roots));

    return roots;
}

// A set of functions to construct a polynomial

template <typename InputIt>
auto make_polynomial_by_roots(InputIt first, InputIt last){
    using CType = typename std::iterator_traits<InputIt>::value_type;

    Polynomial<CType> ret{CType(1)};
    for (; first != last; ++first)
        ret *= Polynomial<CType>{-CType(*first), CType(1)};

    return ret;
}

template <typename CType, template<typename...> typename Cont>
auto make_polynomial_by_roots(const Cont<CType>& v){
    return make_polynomial_by_roots(v.begin(), v.end());
}

template <typename CType>
auto make_polynomial_by_roots(std::initializer_list<CType> l){
    return make_polynomial_by_roots(l.begin(), l.end());
}

namespace lagrange_aux{
    // If it is not in [first, last), then behaviour is undefined
    // If two elements in [first, last) are equal, then behaviour is undefined
    template <typename InputIt>
    auto lagrange_base (InputIt first, InputIt last, InputIt it){
        using CType = typename std::iterator_traits<InputIt>::value_type;
        Polynomial<CType> p{CType(1)};

        for (; first != it; ++first)
            p *= Polynomial<CType>{- *first, CType(1)}/(*it- *first);

        for (++first; first != last; ++first)
            p *= Polynomial<CType>{- *first, CType(1)}/(*it- *first);

        return p;
    }

    // If nodes.size() <= i, then behaviour is undefined
    // If nodes[j] == nodes[k] for some j != k, then behaviour is undefined
    template <typename CType, template<typename...> typename Cont>
    auto lagrange_base (const Cont<CType>& nodes, typename Cont<CType>::size_type i){
        Polynomial<CType> p{CType(1)};

        for (decltype(i) j=0; j < i; ++j)
            p *= Polynomial<CType>{-nodes[j], CType(1)}/(nodes[i]-nodes[j]);

        for (decltype(i) j=i+1; j < nodes.size(); ++j)
            p *= Polynomial<CType>{-nodes[j], CType(1)}/(nodes[i]-nodes[j]);

        return p;
    }
}

// If distance(first1, last1) != distance(first2, last2), then behaviour is undefined
template <typename InputIt1, typename InputIt2>
auto lagrange_polynomial (InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2){
    using CommonType = std::common_type_t<traits_type<InputIt1>, traits_type<InputIt2>>;
    Polynomial<CommonType> p{CommonType(0)};

    for (auto it = first1; first2 != last2; ++it, ++first2)
        p += CommonType(*first2) * Polynomial<CommonType>{std::move(lagrange_aux::lagrange_base(first1, last1, it))};

    return p;
}

// If vx.size() != vy.size(), then behaviour is undefined
template <typename DType, typename RType, template<typename...> typename Cont1, template<typename...> typename Cont2>
auto lagrange_polynomial (const Cont1<DType>& vx, const Cont2<RType>& vy){
    using CommonType = std::common_type_t<DType, RType>;
    Polynomial<CommonType> p{CommonType(0)};

    for (typename Cont2<RType>::size_type i=0; i < vy.size(); ++i)
        p += CommonType(vy[i]) * Polynomial<CommonType>{std::move(lagrange_aux::lagrange_base(vx, i))};

    return p;
}

// If vx.size() != vy.size(), then behaviour is undefined
template <typename DType, typename RType>
auto lagrange_polynomial (std::initializer_list<DType> vx, std::initializer_list<RType> vy){
    return lagrange_polynomial(vx.begin(), vx.end(), vy.begin(), vy.end());
}

// If vx.size() != vy.size(), then behaviour is undefined
template <typename DType, typename RType, template<typename...> typename Cont>
auto lagrange_polynomial (const Cont<DType>& vx, std::initializer_list<RType> vy){
    return lagrange_polynomial(vx.begin(), vx.end(), vy.begin(), vy.end());
}

// If vx.size() != vy.size(), then behaviour is undefined
template <typename DType, typename RType, template<typename...> typename Cont>
auto lagrange_polynomial (std::initializer_list<DType> vx, const Cont<RType>& vy){
    return lagrange_polynomial(vx.begin(), vx.end(), vy.begin(), vy.end());
}

template <typename DType, typename Func, template<typename...> typename Cont = std::vector,
            typename = std::enable_if_t<std::is_invocable_v<Func, DType>>>
auto lagrange_polynomial (const Cont<DType>& nodes, Func&& f){
    std::vector<std::invoke_result_t<Func, DType>> img(nodes.size());
    std::transform(nodes.begin(), nodes.end(), img.begin(), std::forward<Func>(f));

    return lagrange_polynomial(nodes, img);
}

template <typename InputIt, typename Func,
            typename = std::enable_if_t<std::is_invocable_v<Func, traits_type<InputIt>>>>
auto lagrange_polynomial (InputIt first, InputIt last, Func&& f){
    std::vector<std::invoke_result_t<Func, traits_type<InputIt>>> img(std::distance(first, last));
    std::transform(first, last, img.begin(), std::forward<Func>(f));

    return lagrange_polynomial(first, last, img.begin(), img.end());
}

template <typename DType, typename Func,
            typename = std::enable_if_t<std::is_invocable_v<Func, DType>>>
auto lagrange_polynomial (std::initializer_list<DType> nodes, Func&& f){
    return lagrange_polynomial(nodes.begin(), nodes.end(), std::forward<Func>(f));
}

namespace taylor_aux{
    constexpr unsigned long factorial (unsigned long n){
        return (n == 0) ? 1 : n*factorial(n-1);
    }
}

template <typename DType, typename InputIt>
auto taylor_polynomial(DType a, InputIt first, InputIt last){
    using CommonType = std::common_type_t<DType, typename std::iterator_traits<InputIt>::value_type>;

    Polynomial<CommonType> ret{CommonType(*first)}, power{CommonType(1)}, aux{CommonType(-a), CommonType(1)};
    for (++first; first != last; ++first){
        power *= aux;
        ret += power*(CommonType(*first)/CommonType(taylor_aux::factorial(power.degree())));
    }

    return ret;
}

template <typename DType, typename RType, template<typename...> typename Cont>
auto taylor_polynomial(DType a, const Cont<RType>& v){
    return taylor_polynomial(a, v.begin(), v.end());
}

template <typename DType, typename RType>
auto taylor_polynomial(DType a, std::initializer_list<RType> l){
    return taylor_polynomial(a, l.begin(), l.end());
}

template <typename CType>
auto n_root_polynomial(unsigned n, CType a){
    return Polynomial<CType>{0, 1}.pow(n) - Polynomial<CType>{a};
}

template <typename CType>
auto cyclotomic_polynomial(unsigned n){
    if (n == 1)
        return Polynomial<CType>{-1, 1};

    Polynomial<CType> ret{n_root_polynomial(n, CType(1))};

    for (unsigned i=1; i<=n/2; ++i)
        if (n%i == 0)
            ret /= cyclotomic_polynomial<CType>(i);

    return ret;
}

template <typename CType>
auto monic_polynomial(const Polynomial<CType> &pol){
    return pol / pol.last();
}

template <typename CType>
auto reduced_polynomial(const Polynomial<CType> &pol){
    return monic_polynomial(pol)(Polynomial<CType>{-(pol[pol.degree()-1]/pol.last())/CType(pol.degree()), 1});
}

// Typedefs for coefficients in floating point (real numbers)
using polynomial_float          = Polynomial<float>;
using polynomial_double         = Polynomial<double>;
using polynomial_long_double    = Polynomial<long double>;

// Typedefs for coefficients in complex numbers
using polynomial_complex_float          = Polynomial<std::complex<float>>;
using polynomial_complex_double         = Polynomial<std::complex<double>>;
using polynomial_complex_long_double    = Polynomial<std::complex<long double>>;

// Default typedefs
using polynomial            = polynomial_double;
using polynomial_complex    = polynomial_complex_double;

}  // namespace fgs
