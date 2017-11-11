/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functions.hpp>
#include <actl/geometry/algorithm/distance/point_point.hpp>
#include <actl/geometry/sphere.hpp>

namespace ac {

template <class DistancePolicy = comparable_distance_point_point<>>
struct crosses_sphere_sphere : DistancePolicy {};

template <class DP, int N, class T0, class T1>
inline bool crosses(const crosses_sphere_sphere<DP>& policy, const sphere<N, T0>& lhs,
                    const sphere<N, T1>& rhs) {
    auto centers_dist = distance(policy, lhs.center, rhs.center);
    return abs(lhs.radius - rhs.radius) <= centers_dist && centers_dist <= lhs.radius + rhs.radius;
}

template <class DP, int N, class T0, class T1>
inline bool crosses(use_default, const sphere<N, T0>& lhs, const sphere<N, T1>& rhs) {
    return crosses(crosses_sphere_sphere<>(), lhs, rhs);
}

}  // namespace ac
