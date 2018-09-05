/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/2d/point2d.hpp>
#include <actl/geometry/traits/aliases.hpp>
#include <actl/geometry/traits/policy.hpp>
#include <actl/math.hpp>

namespace ac {

template <class S = use_default>
struct rotate_point2d : geometry::policy {};

template <class S, class T0, class T1, class X = geometry::sqrt_t<S, T0, T1>>
inline auto rotate(rotate_point2d<S>, const point<T0>& point, const T1& angle) {
    auto sin = math::sin(static_cast<X>(angle));
    auto cos = math::cos(static_cast<X>(angle));
    return make_point(cos * point[0] - sin * point[1],
                      sin * point[0] + cos * point[1]);
}

template <class S, class T0, class T1, class T2>
inline auto rotate(rotate_point2d<S> policy, const point<T0>& point, const T1& angle,
                   const ac::point<T2>& origin) {
    return origin + rotate(policy, point - origin, angle);
}

template <class T0, class... Ts>
inline auto rotate(use_default, const point<T0>& point, const Ts&... args) {
    return rotate(rotate_point2d<>(), point, args...);
}

}  // namespace ac
