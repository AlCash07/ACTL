/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>
#include <actl/math.hpp>

namespace ac {

template <class P = use_default, class S = use_default,
          class AreaPolicy = standard_area_points<P, S>>
struct standard_angle : AreaPolicy {};

/**
 * Counter-clockwise angle.
 */
template <class P, class S, class AP, int N, class T, class X = geometry::sqrt_t<S, T>>
inline auto angle(standard_angle<P, S, AP>, const point<N, T>& point) {
    return math::atan2(static_cast<X>(point[1]), static_cast<X>(point[0]));
}

template <class P, class S, class AP, int N, class T0, class T1>
inline auto angle(const standard_angle<P, S, AP>& policy, const point<N, T0>& lhs,
                  const point<N, T1>& rhs) {
    return math::atan2(area(policy, lhs, rhs),
                       static_cast<geometry::sqrt_t<S, T0, T1>>(dot<P>(lhs, rhs)));
}

template <class P, class S, class AP, int N, class T0, class T1, class T2>
inline auto angle(const standard_angle<P, S, AP>& policy, const point<N, T0>& lhs,
                  const point<N, T1>& rhs, const point<N, T2>& origin) {
    return angle(policy, lhs - origin, rhs - origin);
}

template <int N, class T, class... Ts>
inline auto angle(const point<N, T>& point, const Ts&... args) {
    return angle(standard_angle<>(), point, args...);
}

}  // namespace ac
