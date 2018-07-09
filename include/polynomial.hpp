#pragma once

#include "aux.hpp"

#include <iostream>  // std::cout, std::cin, std::endl
#include <vector> // std::vector
#include <algorithm> // std::transform, std::generate, std::max_element
#include <initializer_list>   // std::initializer_list
#include <iterator>  // std::iterator_traits
#include <utility>   // std::move
#include <complex>   // std::complex, std::polar
#include <random>   // std::random_device, std::mt19937, std::uniform_real_distribution
#include <exception>    // std::out_of_range
#include <cmath>  // std::pow, std::acos, std::abs

namespace fgs{

    // Alias to get the category of an iterator (random access, bidirectional,...)
    template <typename Iterator>
    using traits_category = typename std::iterator_traits<Iterator>::iterator_category;
    // Alias to get the value type pointed by an iterator
    template <typename Iterator>
    using traits_type = typename std::iterator_traits<Iterator>::value_type;

    // Class Polynomial. CType is the type of the coefficients
    // The container
    template <typename CType>
    class Polynomial{

    private:
        using Container = std::vector<CType>;

        Container coeffs;  // Actual coefficients of the polynomial
        char var = 'x';   // Letter that identifies the variable

        // Helper function to adjust the degree, so the last coefficient is not 0
        void adjust_degree (){
            while (coeffs.back() == value_type(0) && coeffs.size() > 1)
                coeffs.pop_back();
        }

        // Helper constructor for input iterators acceptance
        template <typename InputIt>
        Polynomial (InputIt first, InputIt last, std::input_iterator_tag)
            : coeffs(first, last) {
            adjust_degree();
        }

    public:

        // We make every template specialization a friend class
        template <typename CType2>
        friend class Polynomial;

        // typedefs for the member types (just get the ones from the
        // underlined container).
        using value_type                = typename Container::value_type;
        using allocator_type            = typename Container::allocator_type;
        using size_type                 = typename Container::size_type;
        using difference_type           = typename Container::difference_type;
        using reference                 = typename Container::reference;
        using const_reference           = typename Container::const_reference;
        using pointer                   = typename Container::pointer;
        using const_pointer             = typename Container::const_pointer;
        using iterator                  = typename Container::iterator;
        using const_iterator            = typename Container::const_iterator;
        using reverse_iterator          = typename Container::reverse_iterator;
        using const_reverse_iterator    = typename Container::const_reverse_iterator;

        // Some constructors
        explicit Polynomial ()
            : coeffs(1, value_type(0)) {}
        template <typename U>
        explicit Polynomial (const U& x)
            : coeffs(1, value_type(x)) {}
        Polynomial (std::initializer_list<value_type> l)
            : coeffs(l) {
            adjust_degree();
        }

        // Range constructor using tag dispatching for input iterators
        template <typename InputIt>
        Polynomial (InputIt first, InputIt last)
            : Polynomial(first, last, traits_category<InputIt>()) {}

        // Constructor using the elements of a container (in the future
        // also ranges will be accepted)
        template <template<typename...> typename Cont, typename ...TArgs,
                    typename = std::enable_if_t<aux::is_iterable_v<Cont<TArgs...>>>>
        explicit Polynomial (const Cont<TArgs...>& cont)
            : Polynomial(std::begin(cont), std::end(cont)) {}

        reference operator[] (size_type i){
            return coeffs[i];
        }
        const_reference operator[] (size_type i) const{
            return coeffs[i];
        }

        reference get_coefficient (size_type i) {
            return coeffs.at(i);
        }
        const_reference get_coefficient (size_type i) const{
            return coeffs.at(i);
        }

        reference first() {
            return coeffs.front();
        }
        const_reference first() const{
            return coeffs.front();
        }

        reference last() {
            return coeffs.back();
        }
        const_reference last() const{
            return coeffs.back();
        }

        void set_coefficient (size_type i, const value_type& elem){
            if (i >= coeffs.size())
                coeffs.resize(i+1, value_type(0));

            coeffs[i] = elem;
            adjust_degree();
        }

        char get_variable () const noexcept{
            return var;
        }

        void set_variable (char c) noexcept{
            var = c;
        }

        size_type degree () const noexcept{
            return coeffs.size()-1;
        }

        /* Evaluates the polynomial for the value x using the Horner's
         * polynomial evaluation scheme.
         *
         * RType is the type of the evaluation
         */
        template<typename RType>
        auto evaluate_at (const RType& x) const{
            using Common = std::common_type_t<CType, RType>;
            Common res(coeffs.back());

            for (size_type i=coeffs.size()-1; i>0; --i)
                res = Common(coeffs[i-1]) + res*Common(x);

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
        Polynomial operator+() const{
            return Polynomial(*this);
        }
        Polynomial operator-() const{
            return Polynomial(*this) *= -1;
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

            Container coc(coeffs.size()-pol.coeffs.size()+1, value_type(0));

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
            std::transform(coeffs.begin(), coeffs.end(), coeffs.begin(),
                            [&other](const value_type& val){
                                return val*value_type(other);
                            });
            return *this;
        }
        template <typename U>
        Polynomial& operator/=(const U& other){
            std::transform(coeffs.begin(), coeffs.end(), coeffs.begin(),
                            [&other](const value_type& val){
                                return val/value_type(other);
                            });
            return *this;
        }
        template <typename U>
        Polynomial& operator%=(const U&){
            coeffs.resize(1, 0);
            return *this;
        }

        /* Modifies the coefficients so they match the polynomial to the
         * power of n (n must be unsigned type)
         */
        Polynomial& pow(unsigned n) {
            if (n==0){
                coeffs[0] = value_type(1);
                coeffs.resize(1);
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
            using Common = std::common_type_t<CType, DType, RType>;
            coeffs[0] = value_type(Common(y) - Common((*this).integrate_const().evaluate_at(x)));
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
        operator Polynomial<CType2> () const{   // NOLINT(hicpp-explicit-conversions)
            return Polynomial<CType2>(this->begin(), this->end());
        }

        /* TODO: Decide a good way to input a polynomial (or let the user
         * implement it itself).
         *
         * One idea could be to implement a parser, so it would work for std::cin
         * and a new constructor.
         */
        // friend std::istream& operator>>(std::istream& is, Polynomial& pol);

        /* Pretty print for the polynomial
         *
         * If UNICODE_SUPPORT is enabled, the polynomial will be printed
         * with superscript characters instead of the expresion "^n"
         */
        friend std::ostream& operator<< (std::ostream& os, const Polynomial& pol){
            auto print_coeff = [&os](auto c){
                if constexpr (aux::is_complex_v<decltype(c)>){
                    using v_type = typename decltype(c)::value_type;

                    if (c.imag() != v_type(0)){
                        os << std::noshowpos << "+(" << c.real();
                        if (c.imag() > v_type(0))
                            os << "+i";
                        else
                            os << "-i";
                        os << std::abs(c.imag()) << ")" << std::showpos;
                    }
                    else
                        os << c.real();
                }
                else
                    os << c;
            };

            if (pol.degree() == 0)
                print_coeff(pol.first());
            else{
                os << std::noshowpos;

                if (pol.last() == value_type(-1))
                    os << "-";
                else if (pol.last() != value_type(1))
                    os << pol.last();
                os << pol.get_variable() << aux::exponent_string(pol.degree());

                os << std::showpos;

                for (size_type i=pol.degree()-1; i>0; --i){
                    if (pol[i] != value_type(0)){
                        if (pol[i] == value_type(1))
                            os << "+";
                        else if (pol[i] == value_type(-1))
                            os << "-";
                        else if (pol[i] != value_type(0))
                            print_coeff(pol[i]);

                        os << pol.get_variable() << aux::exponent_string(i);
                    }
                }

                if (pol.first() != value_type(0))
                    print_coeff(pol.first());
            }

            os << std::noshowpos;
            return os;
        }

        /* Iterator functions to iterate through a polynomial
         *
         * It's based on the vector's iterator, so this is just a wrapper
         * for polynomials
         */
        iterator begin(){
            return coeffs.begin();
        }
        const_iterator begin() const{
            return coeffs.begin();
        }

        iterator end(){
            return coeffs.end();
        }
        const_iterator end() const{
            return coeffs.end();
        }

        reverse_iterator rbegin(){
            return coeffs.rbegin();
        }
        const_reverse_iterator rbegin() const{
            return coeffs.rbegin();
        }

        reverse_iterator rend(){
            return coeffs.rend();
        }
        const_reverse_iterator rend() const{
            return coeffs.rend();
        }

        const_iterator cbegin(){
            return coeffs.cbegin();
        }
        const_iterator cend(){
            return coeffs.cend();
        }

        const_reverse_iterator crbegin(){
            return coeffs.crbegin();
        }
        const_reverse_iterator crend(){
            return coeffs.crend();
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
        using Common = std::common_type_t<CType1, CType2>;
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
        [](const Common& a, const Common& b){
            return a == b;
        });
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
        static constexpr double tolerance = 1.0e-6;

        template <typename CType, typename OutputIterator>
        void durand_kerner(const Polynomial<CType>& p, OutputIterator out){
            using RType = std::conditional_t<aux::is_complex_v<CType>, CType, std::complex<CType>>;

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<typename RType::value_type> unif_magnitude(0.0, 1.0 +
                std::abs(*std::max_element(p.begin(), p.end()-1, [](const auto& lhs, const auto& rhs){
                    return std::abs(lhs) < std::abs(rhs);
                })));
            std::uniform_real_distribution<typename RType::value_type> unif_phase(0.0, 4.0*std::acos(0.0));

            auto ret = std::make_pair(std::vector<RType>(p.degree()), std::vector<RType>(p.degree(), 0));
            std::generate(ret.first.begin(), ret.first.end(), [&](){
                return RType(std::polar(unif_magnitude(gen), unif_phase(gen)));
            });

            while ([&ret](){
                for (std::size_t i=0; i<ret.first.size(); ++i)
                    if (std::abs(ret.first[i] - ret.second[i]) >= tolerance)
                        return true;
                return false;
            }()){
                for (std::size_t i=0; i<ret.first.size(); ++i){
                    ret.second[i] = ret.first[i];
                    RType aux(1);

                    for (std::size_t j=0; j<ret.first.size(); ++j)
                        if (i != j)
                            aux *= (ret.second[i] - ret.first[j]);

                    ret.second[i] -= p(ret.first[i]) / aux;
                }
                std::swap(ret.first, ret.second);
            }

            *out = ret.first.front();
            for (auto it = ret.first.begin()+1; it != ret.first.end(); ++it)
                *++out = *it;
        }
    }

    template <typename CType, typename OutputIterator>
    void roots_of(const Polynomial<CType>& p, OutputIterator out){
        using RType = std::conditional_t<aux::is_complex_v<CType>, CType, std::complex<CType>>;

        // clang-format off
        // For polynomials of degree less than 5, we will just use the formula
        if (p.degree() == 1)
            *out = -(p[0]/p[1]);
        else if (p.degree() == 2){
            RType discriminant(std::sqrt(RType(p[1]*p[1] - RType(4)*p[0]*p[2])));
            *out = (-p[1] + discriminant) / (RType(2)*p[2]);
            *++out = (-p[1] - discriminant) / (RType(2)*p[2]);
        }
        /*else if (p.degree() == 3){
            // Taken from https://www.quora.com/Is-there-a-simple-method-of-solving-a-cubic-equation-that-a-16-year-old-student-can-be-taught
            RType Q = (RType(3.0)*p[3]*p[1]-p[2]*p[2])/(RType(9.0)*p[3]*p[3]);
            RType R = (RType(9.0)*p[3]*p[2]*p[1]-RType(27.0)*p[3]*p[3]*p[0]-RType(2.0)*p[2]*p[2]*p[2])/(RType(54.0)*p[3]*p[3]*p[3]);
            RType S = std::pow(R+std::sqrt(Q*Q*Q+R*R), RType(1.0/3.0));
            RType T = std::pow(R-std::sqrt(Q*Q*Q+R*R), RType(1.0/3.0));

            *out = S + T - p[2]/(RType(3.0)*p[3]);
            *++out = -(S+T)/RType(2.0) - p[2]/(RType(3.0)*p[3]) + RType(0.0, std::sqrt(3.0)/2.0)*(S-T);
            *++out = -(S+T)/RType(2.0) - p[2]/(RType(3.0)*p[3]) - RType(0.0, std::sqrt(3.0)/2.0)*(S-T);
        }
        else if (p.degree() == 4){ // FIXME: Wrong calculations, will use Durand-Kerner meanwhile
            // Taken from https://math.stackexchange.com/questions/785/is-there-a-general-formula-for-solving-4th-degree-equations-quartic
            RType p1 = RType(2.0)*p[2]*p[2]*p[2]-RType(9.0)*p[3]*p[2]*p[1]+RType(27.0)*p[4]*p[1]*p[1]+RType(27.0)*p[3]*p[3]*p[0]-RType(72.0)*p[4]*p[2]*p[0];
            RType p2 = p1 + std::sqrt(RType(-4.0)*std::pow(RType(p[2]*p[2]-RType(3.0)*p[3]*p[1]+RType(12.0)*p[4]*p[0]), 3)+p1*p1);
            RType p3 = (p[2]*p[2]-RType(3.0)*p[3]*p[1]+RType(12.0)*p[5]*p[0])/(RType(3.0)*p[4]*std::pow(p2/RType(2.0), RType(1.0/3.0))) + std::pow(p2/RType(2.0), RType(1.0/3.0))/(RType(3.0)*p[4]);
            RType p4 = std::sqrt(RType((p[3]*p[3])/(RType(4.0)*p[4]*p[4])-(RType(2.0)*p[2])/(RType(3.0)*p[4])+p3));
            RType p5 = (p[3]*p[3])/(RType(2.0)*p[4]*p[4])-(RType(4.0)*p[2])/(RType(3.0)*p[4])-p3;
            RType p6 = (-(p[3]*p[3]*p[3])/(p[4]*p[4]*p[4])+(RType(4.0)*p[3]*p[2])/(p[4]*p[4])-(RType(8.0)*p[1])/p[4])/(RType(4.0)*p4);

            *out = -p[3]/(RType(4.0)*p[4])-p4/RType(2.0)-std::sqrt(p5-p6)/RType(2.0);
            *++out = -p[3]/(RType(4.0)*p[4])-p4/RType(2.0)+std::sqrt(p5-p6)/RType(2.0);
            *++out = -p[3]/(RType(4.0)*p[4])+p4/RType(2.0)-std::sqrt(p5+p6)/RType(2.0);
            *++out = -p[3]/(RType(4.0)*p[4])+p4/RType(2.0)+std::sqrt(p5+p6)/RType(2.0);
        }*/
        else if (p.degree() >= 3)
            // For polynomials of degree higher than 3, we will use Durand-Kerner method
            roots_aux::durand_kerner(p/p.last(), out);
        // clang-format on
    }

    template <typename CType, template <typename...> typename Cont = std::vector>
    auto roots_of (const Polynomial<CType>& pol) {
        using RType = std::conditional_t<aux::is_complex_v<CType>, CType, std::complex<CType>>;

        Cont<RType> roots;
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
        using Common = std::common_type_t<traits_type<InputIt1>, traits_type<InputIt2>>;
        Polynomial<Common> p{Common(0)};

        for (auto it = first1; first2 != last2; ++it, ++first2)
            p += Common(*first2) * Polynomial<Common>{std::move(lagrange_aux::lagrange_base(first1, last1, it))};

        return p;
    }

    // If vx.size() != vy.size(), then behaviour is undefined
    template <typename DType, typename RType, template<typename...> typename Cont1, template<typename...> typename Cont2>
    auto lagrange_polynomial (const Cont1<DType>& vx, const Cont2<RType>& vy){
        using Common = std::common_type_t<DType, RType>;
        Polynomial<Common> p{Common(0)};

        for (typename Cont2<RType>::size_type i=0; i < vy.size(); ++i)
            p += Common(vy[i]) * Polynomial<Common>{std::move(lagrange_aux::lagrange_base(vx, i))};

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
        using Common = std::common_type_t<DType, typename std::iterator_traits<InputIt>::value_type>;

        Polynomial<Common> ret{Common(*first)}, power{Common(1)}, aux{Common(-a), Common(1)};
        for (++first; first != last; ++first){
            power *= aux;
            ret += power*(Common(*first)/Common(taylor_aux::factorial(power.degree())));
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

    // Typedefs for coefficients in floating point (real numbers)
    using polynomial_float          = Polynomial<float>;
    using polynomial_double         = Polynomial<double>;
    using polynomial_long_double    = Polynomial<long double>;

    // Typedefs for coefficients in complex numbers
    using polynomial_complex_float          = Polynomial<std::complex<float>>;
    using polynomial_complex_double         = Polynomial<std::complex<double>>;
    using polynomial_complex_long_double    = Polynomial<std::complex<long double>>;

    using polynomial = polynomial_double;

}  // namespace fgs
