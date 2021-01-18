// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/utility/index.hpp>

namespace ac {

namespace detail {

template <index N>
struct max_v : index_constant<N> {};

template <index N, index M>
constexpr auto operator||(max_v<N>, max_v<M>) {
    return max_v < N < M ? M : N > {};
}

}  // namespace detail

template <class T>
struct nesting_depth : index_constant<0> {};

template <class T>
constexpr index nesting_depth_v = nesting_depth<T>::value;

template <template <class...> class T, class... Ts>
struct nesting_depth<T<Ts...>>
    : index_constant<1 + (... || detail::max_v<nesting_depth_v<Ts>>{}).value> {};

}  // namespace ac
