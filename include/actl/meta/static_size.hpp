// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/constant.hpp>
#include <utility> // for std::tuple_size

namespace ac {

inline constexpr size_t dynamic_size = size_t(-1);

template <class T, class = void>
struct static_size : size_constant<dynamic_size>
{};

template <class T>
struct static_size<T const> : static_size<T>
{};

template <class T>
struct static_size<T&> : static_size<T>
{};

template <class T>
struct static_size<
    T,
    std::void_t<decltype(std::tuple_size<std::remove_cv_t<T>>{})>>
    : constant<std::tuple_size<std::remove_cv_t<T>>::value>
{};

template <class T, size_t N>
struct static_size<T[N]> : size_constant<N>
{};

template <class T, size_t N>
struct static_size<T const[N]> : static_size<T[N]>
{};

template <class T>
inline constexpr size_t static_size_v = static_size<T>::value;

/// Static sizes match if they are equal, or one of them is a dynamic_size.
inline constexpr bool static_sizes_match(size_t lhs, size_t rhs) noexcept
{
    return lhs == dynamic_size || rhs == dynamic_size || lhs == rhs;
}

template <class Lhs, class Rhs>
inline constexpr bool have_matching_static_sizes_v =
    static_sizes_match(static_size_v<Lhs>, static_size_v<Rhs>);

} // namespace ac
