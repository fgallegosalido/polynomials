#pragma once

#include <complex>      // std::complex
#include <string>       // std::string, std::to_string
#include <type_traits>  // std::true_type, std::false_type

namespace fgs::detail{
    /* Function that returns any unsigned number in superscript (exponents)
     *
     * If UNICODE_SUPPORT is not enabled, it will return the string "^n"
     */
    template <typename Integral>
    inline std::string exponent (Integral n){
        std::string exponent;

#ifdef UNICODE_SUPPORT
        const std::string::size_type pos = (n < Integral(0))
            ? exponent += "-", n=-n, 1
            : 0;

        while (n > 0){
            switch (n%10) {
                case 0: exponent.insert(pos, "\u2070"); break;
                case 1: exponent.insert(pos, "\u00B9"); break;
                case 2: exponent.insert(pos, "\u00B2"); break;
                case 3: exponent.insert(pos, "\u00B3"); break;
                case 4: exponent.insert(pos, "\u2074"); break;
                case 5: exponent.insert(pos, "\u2075"); break;
                case 6: exponent.insert(pos, "\u2076"); break;
                case 7: exponent.insert(pos, "\u2077"); break;
                case 8: exponent.insert(pos, "\u2078"); break;
                case 9: exponent.insert(pos, "\u2079"); break;
            }
            n /= 10;
        }
#else
        exponent = "^" + std::to_string(n);
#endif

        return exponent;
    }

    // Type trait to check if a type is an instantiation of std::complex
    template<typename T>
    struct is_complex : std::false_type {};
    template<typename T>
    struct is_complex<std::complex<T>> : std::true_type {};
    template<typename T>
    inline constexpr bool is_complex_v = is_complex<T>::value;

    // Type trait that gets the value_type of T in the case T is complex, and T otherwise
    template <typename T>
    struct reduce_complex { using type = T; };
    template <typename T>
    struct reduce_complex<std::complex<T>> { using type = T; };
    template <typename T>
    using reduce_complex_t = typename reduce_complex<T>::type;

    // Type trait to check if a type is iterable
    template <typename T, typename = void>
    struct is_iterable : std::false_type {};
    template <typename T>
    struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                      decltype(std::end(std::declval<T>()))>>
        : std::true_type {};
    template<typename T>
    inline constexpr bool is_iterable_v = is_iterable<T>::value;

}  // namespace fgs::aux
