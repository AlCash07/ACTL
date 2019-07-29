/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/2d/point2d.hpp>
#include <actl/geometry/algorithm/ccw/ccw.hpp>

namespace ac {

/**
 * Returns -1 if rotation rhs <- lhs is counter-clockwise, 0 if collinear, +1 if clockwise.
 */
template <class Policy, class T0, class T1, enable_int_if_policy<Policy> = 0>
inline constexpr int ccw(const Policy& policy, const point<T0>& lhs, const point<T1>& rhs) {
    return sgn(policy, area(policy, rhs, lhs));
}

template <class Policy, class T0, class T1, class T2, enable_int_if_policy<Policy> = 0>
inline constexpr int ccw(const Policy& policy, const point<T0>& lhs, const point<T1>& rhs,
                         const point<T2>& origin) {
    return ccw(policy, lhs - origin, rhs - origin);
}

}  // namespace ac
