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
template <class AP, class T0, class T1>
inline constexpr int ccw(const ccw_policy<AP>& policy, const point2d<T0>& lhs,
                         const point2d<T1>& rhs) {
    return sgn(area(policy, rhs, lhs));
}

template <class AP, class T0, class T1, class T2>
inline constexpr int ccw(const ccw_policy<AP>& policy, const point2d<T0>& lhs,
                         const point2d<T1>& rhs, const point2d<T2>& origin) {
    return ccw(policy, lhs - origin, rhs - origin);
}

}  // namespace ac
