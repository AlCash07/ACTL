/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functions.hpp>
#include <actl/geometry/point.hpp>
#include <actl/geometry/traits/aliases.hpp>
#include <actl/geometry/traits/policy.hpp>

namespace ac {

template <class P = use_default, class R = use_default>
struct invert_policy : geometry::policy {};

template <class P, class R, int N, class T0, class T1, class X = geometry::ratio_t<R, T0, T1>>
inline point<N, X> invert(invert_policy<P, R>, const point<N, T0>& src, const T1& radius) {
    if (!src) return {};
    return src * sqr(static_cast<X>(radius)) / abs<P>(src);
}

template <class P, class R, class S, int N, class T0, class T1, class T2>
inline auto invert(const invert_policy<P, R>& policy, const point<N, T0>& src, const T1& radius,
                   const point<N, T2>& origin) {
    return origin + invert(policy, src - origin, radius);
}

template <int N, class T, class... Ts>
inline auto invert(const point<N, T>& point, const Ts&... args) {
    return invert(invert_policy<>(), point, args...);
}

}  // namespace ac
