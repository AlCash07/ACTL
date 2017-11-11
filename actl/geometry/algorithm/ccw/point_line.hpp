/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/ccw/point_point.hpp>
#include <actl/geometry/line.hpp>

namespace ac {

template <class AP, class T0, class T1, class K>
inline constexpr int ccw(const ccw_policy<AP>& policy, const point2d<T0>& point,
                         const line<2, T1, K>& line) {
    return ccw(policy, point - line.start, line.slope);
}

}  // namespace ac
