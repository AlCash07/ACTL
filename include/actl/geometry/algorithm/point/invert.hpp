/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

template <class P = use_default, class F = use_default>
struct invert_policy : geometry::policy {};

template <class P, class F, index N, class T0, class T1, class X = geometry::float_t<F, T0, T1>>
inline point<X, N> invert(invert_policy<P, F>, const point<T0, N>& src, const T1& radius) {
    if (!src) return {};
    return src * sqr(static_cast<X>(radius)) / dot(default_policy, src);
}

template <class P, class F, index N, class T0, class T1, class T2>
inline auto invert(const invert_policy<P, F>& policy, const point<T0, N>& src, const T1& radius,
                   const point<T2, N>& origin) {
    return origin + invert(policy, src - origin, radius);
}

template <index N, class T, class... Ts>
inline auto invert(const point<T, N>& point, const Ts&... args) {
    return invert(invert_policy{}, point, args...);
}

}  // namespace ac
