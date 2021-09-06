// Extension of the standard type traits.
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac {

// Workaround for CWG 1558: https://en.cppreference.com/w/cpp/types/void_t
template <class... Ts>
struct make_void
{
    using type = void;
};

template <class... Ts>
using void_t = typename make_void<Ts...>::type;

template <bool B, class T>
struct add_const_if : std::conditional<B, const T, T>
{};

template <bool B, class T>
using add_const_if_t = typename add_const_if<B, T>::type;

template <class T>
struct is_integral_constant : std::false_type
{};

template <class T, T N>
struct is_integral_constant<std::integral_constant<T, N>> : std::true_type
{};

template <class T>
inline constexpr bool is_integral_constant_v = is_integral_constant<T>::value;

// TODO: use std::remove_cvref_t when C++20 is out.
template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <bool B>
using enable_int_if = std::enable_if_t<B, int>;

template <class T>
inline constexpr bool is_trivially_moveable_v =
    std::is_trivially_move_constructible_v<T>&&
        std::is_trivially_move_assignable_v<T>;

template <class T, class... Ts>
inline constexpr bool is_one_of_v = (... || std::is_same_v<T, Ts>);

template <class T>
inline constexpr bool is_char_v =
    is_one_of_v<T, char, wchar_t, char16_t, char32_t>;

template <class... Ts>
struct are_same : std::false_type
{};

template <class T>
struct are_same<T> : std::true_type
{};

template <class T, class... Ts>
struct are_same<T, T, Ts...> : are_same<T, Ts...>
{};

template <class... Ts>
inline constexpr bool are_same_v = are_same<Ts...>::value;

template <class T, class = void>
struct is_equality_comparable : std::false_type
{};

template <class T>
struct is_equality_comparable<
    T,
    std::void_t<decltype(
        std::declval<T>() == std::declval<T>(),
        std::declval<T>() != std::declval<T>())>> : std::true_type
{};

template <class T>
inline constexpr bool is_equality_comparable_v =
    is_equality_comparable<T>::value;

} // namespace ac
