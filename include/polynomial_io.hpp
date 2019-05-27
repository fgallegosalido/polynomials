#include <iostream>
#include <sstream>

#include "detail.hpp"
#include "polynomial.hpp"

namespace fgs{

/* TODO: Decide a good way to input a polynomial (or let the user
 * implement it itself).
 *
 * One idea could be to implement a parser, so it would work for std::cin
 * and a new constructor.
 */
// std::istream& operator>>(std::istream& is, Polynomial& pol);

/* Pretty print for the polynomial
 *
 * If UNICODE_SUPPORT is enabled, the polynomial will be printed
 * with superscript characters instead of the expresion "^n"
 */
template <typename CType, typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<< (std::basic_ostream<charT, traits> &os, const Polynomial<CType> &pol){
    std::basic_ostringstream<charT, traits> s;
    s.flags(os.flags());
    s.precision(os.precision());

    if (pol.degree() == 0)
        s << pol[0];
    else{
        if constexpr (detail::is_complex_v<CType>){
            if (pol[0] != CType(0))
                s << pol[0];
            if (pol[1] != CType(0))
                s << "+" << pol[1] << pol.get_variable();

            for (typename Polynomial<CType>::size_type i=2; i<=pol.degree(); ++i)
                if (pol[i] != CType(0))
                    s << "+" << pol[i] << pol.get_variable() << detail::exponent(i);
        }
        else{
            if (pol[0] != CType(0))
                s << pol[0];
            s << std::showpos;
            if (pol[1] != CType(0))
                s << pol[1] << pol.get_variable();

            for (typename Polynomial<CType>::size_type i=2; i<=pol.degree(); ++i)
                if (pol[i] != CType(0))
                    s << pol[i] << pol.get_variable() << detail::exponent(i);
        }
    }

    return os << s.str();
}

}   // namespace fgs
