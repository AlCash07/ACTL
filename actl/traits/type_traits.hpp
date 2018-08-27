/***************************************************************************************************
 * Extension of the standard type traits.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <type_traits>

namespace ac {

// TODO: use std::is_invocable what C++17 is fully supported.
template <class, class = void>
struct is_invocable : std::false_type {};

template <class F, class... Ts>
struct is_invocable<F(Ts...), void_t<decltype(std::declval<F>()(std::declval<Ts>()...))>>
    : std::true_type {};

template <class F, class... Ts>
inline constexpr bool is_invocable_v = is_invocable<F(Ts...)>::value;

// TODO: use std::remove_cvref_t when C++20 is out.
template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <int N>
using int_constant = std::integral_constant<int, N>;

template <class T>
struct is_signed_int : std::bool_constant<std::is_signed_v<T> && std::is_integral_v<T>> {};

template <class T>
struct is_unsigned_int : std::bool_constant<std::is_unsigned_v<T> && std::is_integral_v<T>> {};

template <class T>
struct is_non_const_reference
    : std::bool_constant<std::is_reference_v<T> && !std::is_const_v<std::remove_reference_t<T>>> {};

template <class T> struct remove_rvalue_reference      { using type = T; };
template <class T> struct remove_rvalue_reference<T&&> { using type = T; };

template <class T>
using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;

template <bool B, class T>
struct add_const_if : std::conditional<B, const T, T> {};

template <bool B, class T>
using add_const_if_t = typename add_const_if<B, T>::type;

template <class From, class To, class = void>
struct is_static_castable : std::false_type {};

template <class From, class To>
struct is_static_castable<From, To, std::void_t<decltype(static_cast<To>(std::declval<From>()))>>
    : std::true_type {};

}  // namespace ac
