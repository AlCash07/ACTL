// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/constant.hpp>

namespace ac {

/// Nesting depth specifies the maximum number of nested composite types
/// (such as tuples and ranges) in the given type @p T.
/// For example, nesting depth of `std::tuple<int, std::vector<int>>` is 2.
template<class T>
struct nesting_depth : size_constant<0> {};

template<class T>
inline constexpr size_t nesting_depth_v = nesting_depth<T>::value;

namespace detail {

template<size_t N>
struct max_v : constant<N> {};

// Operator overloading is used, because it's the only way to use standard
// fold expressions.
template<size_t N, size_t M>
constexpr auto operator||(max_v<N>, max_v<M>) {
    return max_v < (N < M) ? M : N > {};
}

} // namespace detail

template<class... Ts>
struct max_nesting_depth
    : size_constant<(... || detail::max_v<nesting_depth_v<Ts>>{})> {};

template<class... Ts>
inline constexpr size_t max_nesting_depth_v = max_nesting_depth<Ts...>::value;

} // namespace ac
