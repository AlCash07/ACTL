/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/crosses/crosses.hpp>
#include <actl/geometry/algorithm/distance/point_point.hpp>
#include <actl/geometry/sphere.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1>
inline bool crosses(const Policy& policy, const sphere<T0, N>& lhs, const sphere<T1, N>& rhs) {
    auto centers_dist = distance(policy, lhs.center, rhs.center);
    return !less(policy, centers_dist, op::abs(lhs.radius - rhs.radius)) &&
           !less(policy, lhs.radius + rhs.radius, centers_dist);
}

}  // namespace ac
