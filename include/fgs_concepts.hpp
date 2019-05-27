#include <type_traits>

#ifdef __GNUG__
    #define FGS_CONCEPT_DEFINITION concept bool
#else
    #define FGS_CONCEPT_DEFINITION concept
#endif

namespace fgs::concepts{
    #ifdef __cpp_­lib_­concepts
    template <typename T>
    FGS_CONCEPT_DEFINITION Integral = std::is_integral_v<T>;

    template <typename T>
    FGS_CONCEPT_DEFINITION SignedIntegral = Integral<T> && std::is_signed_v<T>;

    template <typename T>
    FGS_CONCEPT_DEFINITION UnsignedIntegral = Integral<T> && !SignedIntegral<T>;

    template <typename T>
    FGS_CONCEPT_DEFINITION Destructible = std::is_nothrow_destructible_v<T>;

    template <typename T, typename... Args>
    FGS_CONCEPT_DEFINITION Constructible = Destructible<T> && std::is_constructible_v<T, Args...>;

    template <typename T, typename U>
    FGS_CONCEPT_DEFINITION Same = std::is_same_v<T, U>;

    /*
    template <typename From, typename To>
    FGS_CONCEPT_DEFINITION ConvertibleTo = std::is_convertible_v<From, To> &&
    requires(From (&f)()) {
        static_cast<To>(f());
    };

    template <typename T, typename U>
    FGS_CONCEPT_DEFINITION CommonReference =
        Same<std::common_reference_t<T, U>, std::common_reference_t<U, T>> &&
        ConvertibleTo<T, std::common_reference_t<T, U>> &&
        ConvertibleTo<U, std::common_reference_t<T, U>>;

    template <typename T, typename U>
    FGS_CONCEPT_DEFINITION Common =
        Same<std::common_type_t<T, U>, std::common_type_t<U, T>> &&
        requires {
            static_cast<std::common_type_t<T, U>>(std::declval<T>());
            static_cast<std::common_type_t<T, U>>(std::declval<U>());
        } &&
        CommonReference<
            std::add_lvalue_reference_t<const T>,
            std::add_lvalue_reference_t<const U>> &&
        CommonReference<
            std::add_lvalue_reference_t<std::common_type_t<T, U>>,
            std::common_reference_t<
                std::add_lvalue_reference_t<const T>,
                std::add_lvalue_reference_t<const U>>>;
    */
    #else
        #define Integral            typename
        #define SignedIntegral      typename
        #define UnsignedIntegral    typename
        #define Destructible        typename
        #define Constructible       typename
        #define Same                typename
        /*
        #define ConvertibleTo       typename
        #define CommonReference     typename
        #define Common              typename
        */
    #endif
}
