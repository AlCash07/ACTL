/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>
#include <actl/numeric/math.hpp>

namespace ac {

template <class P = use_default, class F = use_default,
          class AreaPolicy = standard_area_points<P, F>>
struct standard_angle : AreaPolicy {};

/**
 * Counter-clockwise angle.
 */
template <class P, class F, class AP, index N, class T, class X = geometry::float_t<F, T>>
inline auto angle(standard_angle<P, F, AP>, const point<T, N>& point) {
    return math::atan2(static_cast<X>(point[1]), static_cast<X>(point[0]));
}

template <class P, class F, class AP, index N, class T0, class T1>
inline auto angle(const standard_angle<P, F, AP>& policy, const point<T0, N>& lhs,
                  const point<T1, N>& rhs) {
    return math::atan2(area(policy, lhs, rhs),
                       static_cast<geometry::float_t<F, T0, T1>>(dot<P>(lhs, rhs)));
}

template <class P, class F, class AP, index N, class T0, class T1, class T2>
inline auto angle(const standard_angle<P, F, AP>& policy, const point<T0, N>& lhs,
                  const point<T1, N>& rhs, const point<T2, N>& origin) {
    return angle(policy, lhs - origin, rhs - origin);
}

template <index N, class T, class... Ts>
inline auto angle(const point<T, N>& point, const Ts&... args) {
    return angle(standard_angle<>{}, point, args...);
}

}  // namespace ac
