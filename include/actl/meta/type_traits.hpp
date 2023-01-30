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

template <bool B, class T>
struct add_const_if : std::conditional<B, T const, T>
{};

template <bool B, class T>
using add_const_if_t = typename add_const_if<B, T>::type;

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

} // namespace ac
