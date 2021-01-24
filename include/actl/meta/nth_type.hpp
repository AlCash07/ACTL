// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <cstddef>

namespace ac {

template <size_t N, class... Ts>
struct nth_type;

template <class T, class... Ts>
struct nth_type<0, T, Ts...> {
    using type = T;
};

template <size_t N, class T, class... Ts>
struct nth_type<N, T, Ts...> : nth_type<N - 1, Ts...> {};

template <size_t N, class... Ts>
using nth_type_t = typename nth_type<N, Ts...>::type;

} // namespace ac
