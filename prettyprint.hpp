#ifndef PRETTYPRINT_HPP_INCLUDE
#define PRETTYPRINT_HPP_INCLUDE

#include <iterator>    // std::begin std::end
#include <ostream>     // std::basic_ostream
#include <tuple>       // std::tuple std::get
#include <type_traits> // metafunctions
#include <utility>     // std::pair std::index_sequence(c++14)

namespace pprint {

namespace detail {

#if __cplusplus >= 201703L
    using std::void_t;
    using std::disjunction;
#else
    template <class...>
    using void_t = void;

    template <class...>
    struct disjunction : std::false_type {};
    template <class T>
    struct disjunction<T> : T {};
    template <class T, class... Rest>
    struct disjunction<T, Rest...> : std::conditional<(bool)T::value, T, disjunction<Rest...>>::type {};
#endif

    template <class T, class... Args>
    using is_any_of = disjunction<std::is_same<T, Args>...>;

    template <class T>
    using is_character = is_any_of<typename std::decay<T>::type,
#if __cplusplus >= 202002L
                                   char, wchar_t, char16_t, char32_t, char8_t>;
#else
                                   char, wchar_t, char16_t, char32_t>;
#endif

    template <class T, class = void>
    struct is_basic_string_or_view : std::false_type {};
    template <class T>
    struct is_basic_string_or_view<T, void_t<typename T::value_type,
                                             typename T::const_iterator,
                                             typename T::traits_type>> : std::true_type {};

    template <class T, class = void, class = void>
    struct is_container : std::false_type {};
    template <class T>
    struct is_container<T, void_t<typename T::value_type,
                                  typename T::const_iterator,
                                  decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>,
                                  typename std::enable_if<!is_basic_string_or_view<T>::value>::type> : std::true_type {};

    template <class T>
    using remove_all = std::remove_cv<
                       typename std::remove_pointer<
                       typename std::remove_reference<
                       typename std::remove_all_extents<T>::type>::type>::type>;

    template <class T>
    using is_array = std::integral_constant<bool, std::is_array<T>::value &&
                                                  !is_character<typename remove_all<T>::type>::value>;
    //ostream operator<< for array of char is overloaded in std.

} // namespace detail

namespace ptuple {

#if __cplusplus >= 201402L
    using std::index_sequence;
    using std::make_index_sequence;
    using std::index_sequence_for;
#else
    template <std::size_t...>
    struct index_sequence {};

    template <std::size_t N, std::size_t... Is>
    struct make_index_sequence : make_index_sequence<N - 1, N - 1, Is...> {};

    template <std::size_t... Is>
    struct make_index_sequence<0, Is...> : index_sequence<Is...> {};

    template <class... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;
#endif

    template <class Tuple, std::size_t... Is>
    inline void print(std::ostream& os, const Tuple& tuple, index_sequence<Is...>) {
    #if __cplusplus >= 201703L
        ((void)(os << (Is == 0 ? "" : ", ") << std::get<Is>(tuple)), ...);
    #else
        using swallow = char[];
        (void)swallow {'\0', ((void)(os << (Is == 0 ? "" : ", ") << std::get<Is>(tuple)), '\0')...};
    #endif
    }

} // namespace ptuple

} // namespace pprint

template <class CharT, class CharTraits, class Container,
          class = typename std::enable_if<pprint::detail::is_container<Container>::value ||
                                          pprint::detail::is_array<Container>::value>::type>
inline std::basic_ostream<CharT, CharTraits>&
operator<<(std::basic_ostream<CharT, CharTraits>& os, const Container& container) {
    auto itr = std::begin(container);
    const auto end = std::end(container);
    os << '[';
    if (itr != end) {
        while(true) {
            os << *itr;
            if (++itr == end) break;
            os << ", ";
        }
    }
    return os << ']';
}

template <class CharT, class CharTraits, class T, class U>
inline std::basic_ostream<CharT, CharTraits>&
operator<<(std::basic_ostream<CharT, CharTraits>& os, const std::pair<T, U>& pair) {
    return os << '(' << pair.first << ", " << pair.second << ')';
}

template <class CharT, class CharTraits, class... Args>
inline std::basic_ostream<CharT, CharTraits>&
operator<<(std::basic_ostream<CharT, CharTraits>& os, const std::tuple<Args...>& tuple) {
    os << '(';
    pprint::ptuple::print(os, tuple, pprint::ptuple::index_sequence_for<Args...> {});
    return os << ')';
}

#endif // PRETTYPRINT_HPP_INCLUDE