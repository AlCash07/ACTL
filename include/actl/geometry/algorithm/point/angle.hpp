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
template <class Policy, index N, class T, enable_int_if_policy<Policy> = 0>
inline auto angle(Policy&& policy, const point<T, N>& p) {
    return adl::atan2(p[1], p[0]);
}

template <class Policy, index N, class T0, class T1, enable_int_if_policy<Policy> = 0>
inline auto angle(Policy&& policy, const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return adl::atan2(area(policy, lhs, rhs), dot(policy, lhs, rhs));
}

template <class Policy, index N, class T0, class T1, class T2, enable_int_if_policy<Policy> = 0>
inline auto angle(Policy&& policy, const point<T0, N>& lhs, const point<T1, N>& rhs,
                  const point<T2, N>& origin) {
    return angle(policy, lhs - origin, rhs - origin);
}

template <class T, class... Ts, disable_int_if_policy<T> = 0>
inline auto angle(const T& x, const Ts&... xs) {
    return angle(geometry_policy, x, xs...);
}

}  // namespace ac
