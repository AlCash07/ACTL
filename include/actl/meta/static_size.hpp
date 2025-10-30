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

template<typename T>
struct static_size : size_constant<dynamic_size> {};

template<typename T>
struct static_size<T const> : static_size<T> {};

template<typename T>
struct static_size<T&> : static_size<T> {};

template<typename T>
struct static_size<T&&> : static_size<T> {};

template<typename T>
    requires requires { std::tuple_size<std::remove_cv_t<T>>{}; }
struct static_size<T>
    : constant<std::tuple_size<std::remove_cv_t<T>>::value> {};

template<typename T, size_t N>
struct static_size<T[N]> : size_constant<N> {};

template<typename T, size_t N>
struct static_size<T const[N]> : static_size<T[N]> {};

template<typename T>
inline constexpr size_t static_size_v = static_size<T>::value;

template<typename T>
concept HasDynamicSize = static_size_v<T> == dynamic_size;

template<typename T>
concept HasStaticSize = !HasDynamicSize<T>;

/// Static sizes match if they are equal, or one of them is a dynamic_size.
inline constexpr bool static_sizes_match(size_t l, size_t r) noexcept {
    return l == dynamic_size || r == dynamic_size || l == r;
}

template<typename L, typename R>
inline constexpr bool have_matching_static_sizes_v =
    static_sizes_match(static_size_v<L>, static_size_v<R>);

} // namespace ac
