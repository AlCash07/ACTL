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
struct static_size
{
    using type = size_constant<dynamic_size>;
};

template <class T>
struct static_size<T&> : static_size<T>
{};

template <class T>
struct static_size<
    T,
    std::void_t<decltype(std::tuple_size<std::remove_cv_t<T>>{})>>
{
    using type = constant<std::tuple_size<std::remove_cv_t<T>>::value>;
};

template <class T, size_t N>
struct static_size<T[N]>
{
    using type = size_constant<N>;
};

template <class T>
using static_size_t = typename static_size<T>::type;

template <class T>
inline constexpr size_t static_size_v = static_size_t<T>::value;

} // namespace ac
