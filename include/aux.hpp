#pragma once

#include <string> // std::string, std::to_string
#include <algorithm> // std::reverse
#include <limits> // std::numeric_limits
#include <complex>  // std::complex
#include <type_traits>  // std::true_type, std::false_type

namespace fgs::aux{
    namespace unicode{
        /* Array that contains numbers from 0 to 9 (including empty string)
         *
         * If the user defines the macro UNICODE_SUPPORT, an array of
         * superscript/subscript numbers will be generated instead of normal ones
         */
        #ifdef UNICODE_SUPPORT
        constexpr const char* superscript_chars[] = {
        "\u2070", "\u00B9", "\u00B2", "\u00B3", "\u2074", "\u2075", "\u2076", "\u2077", "\u2078", "\u2079", ""};
        constexpr const char* subscript_chars[] = {
        "\u2080", "\u2081", "\u2082", "\u2083", "\u2084", "\u2085", "\u2086", "\u2087", "\u2088", "\u2089", ""};
        #else
        constexpr const char* superscript_chars[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ""};
        constexpr const char* subscript_chars[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ""};
        #endif

        /* Functions that returns a number from 0 to 9 in superscript/subscript
         * (if enabled by the user through the UNICODE_SUPPORT macro)
         */
        constexpr auto superscript (std::size_t n){
            return (n<10) ? superscript_chars[n]
                    : superscript_chars[10];   // Any other number will return an empty string
        }
        constexpr auto subscript (std::size_t n){
            return (n<10) ? subscript_chars[n]
                    : subscript_chars[10];   // Any other number will return an empty string
        }
    }  // namespace unicode

    /* Function that returns any unsigned number in superscript (exponents)
     *
     * If UNICODE_SUPPORT is not enabled, it will return the string "^n"
     */
    inline std::string exponent_string (std::size_t n){
        if (n==1 || n==0)
            return std::string("");

        std::string exponent;

        #ifdef UNICODE_SUPPORT
        while (n>0){
            exponent = unicode::superscript(n%10) + exponent;
            n /= 10;
        }
        #else
        exponent = "^" + std::to_string(n);
        #endif

        return exponent;
    }

    /* Function that returns any unsigned number in subscript (subindex)
     *
     * If UNICODE_SUPPORT is not enabled, it will return the string "_n"
     */
    inline std::string subindex_string (std::size_t n){
        std::string subindex("0");

        #ifdef UNICODE_SUPPORT
        while (n>0){
            subindex = unicode::subscript(n%10) + subindex;
            n /= 10;
        }
        #else
        subindex = "_" + std::to_string(n);
        #endif

        return subindex;
    }

    // Type trait to check if a type is an instantiation of std::complex
    template<typename T> struct is_complex : std::false_type {};
    template<typename T> struct is_complex<std::complex<T>> : std::true_type {};
    template<typename T> inline constexpr bool is_complex_v = is_complex<T>::value;

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
    template<typename T> inline constexpr bool is_iterable_v = is_iterable<T>::value;
    
}  // namespace fgs::aux
