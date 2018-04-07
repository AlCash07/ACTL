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

template <class From, class To, class = void>
struct is_static_castable : std::false_type {};

template <class From, class To>
struct is_static_castable<From, To, std::void_t<decltype(static_cast<To>(std::declval<From>()))>>
    : std::true_type {};

}  // namespace ac
