/***************************************************************************************************
 * Copyright 2018 Roman Rizvanov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/traits/container_traits.hpp>
#include <actl/traits/type_traits.hpp>
#include <iterator>

namespace ac {

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

MAKE_IS_TYPE_TRAIT(is_array);
MAKE_IS_TYPE_TRAIT(is_pair);
MAKE_IS_TYPE_TRAIT(is_tuple);
MAKE_IS_TYPE_TRAIT(is_string);

#undef MAKE_IS_TYPE_TRAIT

template <class T>
static constexpr type_kind type_kind_of = 
    std::is_same<remove_cvref_t<T>, bool>::value ? type_kind::boolean :
    std::is_floating_point<T>::value ? type_kind::floating_point :
    std::is_signed<T>::value ? type_kind::signed_integer :
    std::is_unsigned<T>::value ? type_kind::unsigned_integer :
    is_pair_v<T> ? type_kind::pair :
    is_tuple_v<T> ? type_kind::tuple :
    is_array_v<T> ? type_kind::array :
    is_string_v<T> ? type_kind::string :
    is_sequence_container_v<T> ? type_kind::sequence_container :
    is_simple_associative_container_v<T> ? type_kind::set :
    is_pair_associative_container_v<T> ? type_kind::map : type_kind::unknown;

}  // namespace ac
