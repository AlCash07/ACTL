// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <cstddef>

namespace ac {

template <size_t N, class... Ts>
struct type_at;

template <class T, class... Ts>
struct type_at<0, T, Ts...> {
    using type = T;
};

template <size_t N, class T, class... Ts>
struct type_at<N, T, Ts...> : type_at<N - 1, Ts...> {};

template <size_t N, class... Ts>
using type_at_t = typename type_at<N, Ts...>::type;

} // namespace ac
