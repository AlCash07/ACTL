// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/constant.hpp>
#include <actl/meta/dynamic_size.hpp>
#include <actl/meta/type_traits.hpp>
#include <utility> // for std::tuple_size

namespace ac {

template <class T, class = void>
struct static_size : size_constant<dynamic_size>
{};

template <class T>
struct static_size<T, void_t<decltype(std::tuple_size<std::remove_cv_t<T>>{})>>
    : std::tuple_size<std::remove_cv_t<T>>
{};

template <class T, size_t N>
struct static_size<T[N]> : size_constant<N>
{};

template <class T>
inline constexpr size_t static_size_v = static_size<T>::value;

} // namespace ac
