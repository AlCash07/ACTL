// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <cstddef>
#include <type_traits>

namespace ac {

template <auto X>
struct constant : std::integral_constant<decltype(X), X>
{};

template <bool B>
using bool_constant = constant<B>;

template <bool B>
inline constexpr auto bool_c = bool_constant<B>{};

template <int N>
using int_constant = constant<N>;

template <int N>
inline constexpr auto int_c = int_constant<N>{};

template <size_t N>
using size_constant = constant<N>;

template <size_t N>
inline constexpr auto size_c = size_constant<N>{};

template <class T>
struct is_constant : std::false_type
{};

template <auto X>
struct is_constant<constant<X>> : std::true_type
{};

template <class T>
inline constexpr bool is_constant_v = is_constant<T>::value;

} // namespace ac
