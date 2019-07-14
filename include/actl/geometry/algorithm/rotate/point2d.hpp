/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/2d/point2d.hpp>
#include <actl/numeric/math.hpp>

namespace ac {

template <class F = use_default>
struct rotate_point2d : geometry::policy {};

template <class F, class T0, class T1, class X = geometry::float_t<F, T0, T1>>
inline auto rotate(rotate_point2d<F>, const point<T0>& p, const T1& angle) {
    auto sin = adl::sin(static_cast<X>(angle));
    auto cos = adl::cos(static_cast<X>(angle));
    return point{cos * p[0] - sin * p[1], sin * p[0] + cos * p[1]};
}

template <class F, class T0, class T1, class T2>
inline auto rotate(rotate_point2d<F> policy, const point<T0>& point, const T1& angle,
                   const ac::point<T2>& origin) {
    return origin + rotate(policy, point - origin, angle);
}

template <class T0, class... Ts>
inline auto rotate(use_default, const point<T0>& p, const Ts&... args) {
    return rotate(rotate_point2d{}, p, args...);
}

}  // namespace ac
