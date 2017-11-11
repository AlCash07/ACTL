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

// TODO: switch to std after C++17.
template <typename... Ts> struct make_void { typedef void type; };
template <typename... Ts> using void_t = typename make_void<Ts...>::type;

// TODO: switch to std after C++17.
template <bool B>
using bool_constant = std::integral_constant<bool, B>;

template <int N>
using int_constant = std::integral_constant<int, N>;

template <class T>
struct is_signed_int : bool_constant<std::is_signed<T>::value && std::is_integral<T>::value> {};

template <class T>
struct is_unsigned_int : bool_constant<std::is_unsigned<T>::value && std::is_integral<T>::value> {};

template <class T>
struct is_non_const_reference : bool_constant<std::is_reference<T>::value &&
                                              std::is_const<std::remove_reference_t<T>>::value> {};

template <class T> struct remove_rvalue_reference      { using type = T; };
template <class T> struct remove_rvalue_reference<T&&> { using type = T; };

template <class T>
using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;

}  // namespace ac
