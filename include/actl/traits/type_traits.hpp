// Extension of the standard type traits.
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/types.hpp>
#include <type_traits>

namespace ac {

template <bool B, class T>
struct add_const_if : std::conditional<B, const T, T> {};

template <bool B, class T>
using add_const_if_t = typename add_const_if<B, T>::type;

template <index N>
using index_constant = std::integral_constant<index, N>;

template <class T>
struct is_index_constant : std::false_type {};

template <index N>
struct is_index_constant<index_constant<N>> : std::true_type {};

// TODO: use std::remove_cvref_t when C++20 is out.
template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <bool B>
using enable_int_if = std::enable_if_t<B, int>;

template <class T, class... Ts>
constexpr bool is_one_of_v = (... || std::is_same_v<T, Ts>);

template <class T>
constexpr bool is_char_v = is_one_of_v<T, char, wchar_t, char16_t, char32_t>;

template <class... Ts>
struct are_same : std::false_type {};

template <class T>
struct are_same<T> : std::true_type {};

template <class T, class... Ts>
struct are_same<T, T, Ts...> : are_same<T, Ts...> {};

template <class... Ts>
constexpr bool are_same_v = are_same<Ts...>::value;

}  // namespace ac
