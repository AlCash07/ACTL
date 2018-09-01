#pragma once

#include <iterator>
#include <type_traits>

namespace cp {

enum class type_kind {
    boolean,                // bool
    signed_integer,         // int, int32_t, ...
    unsigned_integer,       // unsigned, uint32_t, ...
    integer,                // signed or unsigned integer
    floating_point,         // float, double, long double
    arithmetic,             // integer or floating point number
    pair,                   // std::pair
    tuple,                  // std::tuple
    string,                 // std::string, char*, ...
    set,                    // std::set, std::unordered_set
    map,                    // std::map, std::unordered_map
    array,                  // T[], std::array
    sequence_container,     // std::vector, std::list, ...
    associative_container,  // map or set
    container,              // sequence or associative container
    unknown
};

namespace detail {

template <class Lhs, class Rhs>
using static_and = std::integral_constant<bool, Lhs::value && Rhs::value>;

template <class Lhs, class Rhs>
using static_or = std::integral_constant<bool, Lhs::value || Rhs::value>;

template <class Lhs, class Rhs>
using static_xor = std::integral_constant<bool, Lhs::value != Rhs::value>;

// is_container

template <class T, class = decltype(std::begin(std::declval<T>())), class = decltype(std::end(std::declval<T>()))>
std::true_type is_container_impl(int);

template <class T>
std::false_type is_container_impl(...);

template <class T>
using is_container = static_or<decltype(is_container_impl<T>(0)), std::is_array<T>>;

// is_associative_container

template <class T, class = typename T::key_type, class = typename T::value_type>
std::true_type is_associative_container_impl(int);

template <class T>
std::false_type is_associative_container_impl(...);

template <class T>
using is_associative_container = static_and<is_container<T>, decltype(is_associative_container_impl<T>(0))>;

// is_array

template <class T, typename T::size_type = T().size()>
std::true_type has_static_size(int);

template <class T>
std::false_type has_static_size(...);

template <class T>
struct is_array_impl : std::false_type {};

template <class T, size_t N>
struct is_array_impl<T[N]> : std::true_type {};

template <class T>
using is_array = static_or<is_array_impl<T>, decltype(has_static_size<T>(0))>;

// is_map

template <class T, class = typename T::mapped_type>
std::true_type is_map_impl(int);

template <class T>
std::false_type is_map_impl(...);

template <class T>
using is_map = static_and<is_associative_container<T>, decltype(is_map_impl<T>(0))>;

template <class T>
using is_set = static_xor<is_associative_container<T>, is_map<T>>;

// is_sequence_container

template <class T>
using is_sequence_container = static_xor<is_container<T>, is_associative_container<T>>;

// is_pair

template <class T>
struct is_pair : std::false_type {};

template <class K, class V>
struct is_pair<std::pair<K, V>> : std::true_type {};

// is_tuple

template <class T>
struct is_tuple : std::false_type {};

template <class... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

// is_string

template <class T>
struct is_string : std::false_type {};

template <class C, class T, class A>
struct is_string<std::basic_string<C, T, A>> : std::true_type {};

template <int N>
struct is_string<char[N]> : std::true_type {};

template <>
struct is_string<char[]> : std::true_type {};

template <>
struct is_string<char*> : std::true_type {};

template <>
struct is_string<const char*> : std::true_type {};

}  // namespace detail

// _v

template <type_kind kind>
struct general_type_kind : std::integral_constant<type_kind, type_kind::unknown> {};

#define MAKE_GENERAL_TYPE_KIND(child, parent)        \
    template <> struct general_type_kind<type_kind::child> : std::integral_constant<type_kind, type_kind::parent> {};
 
MAKE_GENERAL_TYPE_KIND(signed_integer, integer);
MAKE_GENERAL_TYPE_KIND(unsigned_integer, integer);
MAKE_GENERAL_TYPE_KIND(integer, arithmetic);
MAKE_GENERAL_TYPE_KIND(floating_point, arithmetic);
MAKE_GENERAL_TYPE_KIND(set, associative_container);
MAKE_GENERAL_TYPE_KIND(map, associative_container);
MAKE_GENERAL_TYPE_KIND(array, sequence_container);
MAKE_GENERAL_TYPE_KIND(sequence_container, container);
MAKE_GENERAL_TYPE_KIND(associative_container, container);

#undef MAKE_GENERAL_TYPE_KIND

template <type_kind kind>
static constexpr type_kind general_type_kind_v = general_type_kind<kind>::value;

// Helper template variables

#define MAKE_IS_TYPE_TRAIT(trait)                                                                 \
    template <class T> using trait = detail::trait<std::remove_cv_t<std::remove_reference_t<T>>>; \
    template <class T> static constexpr auto trait##_v = trait<T>::value

MAKE_IS_TYPE_TRAIT(is_container);
MAKE_IS_TYPE_TRAIT(is_associative_container);
MAKE_IS_TYPE_TRAIT(is_sequence_container);
MAKE_IS_TYPE_TRAIT(is_map);
MAKE_IS_TYPE_TRAIT(is_set);
MAKE_IS_TYPE_TRAIT(is_array);
MAKE_IS_TYPE_TRAIT(is_pair);
MAKE_IS_TYPE_TRAIT(is_tuple);
MAKE_IS_TYPE_TRAIT(is_string);

#undef MAKE_IS_TYPE_TRAIT

template <class T>
static constexpr type_kind type_kind_of = 
    std::is_same<std::decay_t<T>, bool>::value ? type_kind::boolean :
    std::is_floating_point<T>::value ? type_kind::floating_point :
    std::is_signed<T>::value ? type_kind::signed_integer :
    std::is_unsigned<T>::value ? type_kind::unsigned_integer :
    is_pair_v<T> ? type_kind::pair :
    is_tuple_v<T> ? type_kind::tuple :
    is_array_v<T> ? type_kind::array :
    is_string_v<T> ? type_kind::string :
    is_sequence_container_v<T> ? type_kind::sequence_container :
    is_set_v<T> ? type_kind::set :
    is_map_v<T> ? type_kind::map : type_kind::unknown;

}  // namespace cp