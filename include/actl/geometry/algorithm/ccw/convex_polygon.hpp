/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/ccw/point_point.hpp>
#include <actl/geometry/polygon.hpp>

namespace ac {

template <class AP, class T>
inline int ccw(const ccw_policy<AP>& policy, const convex_polygon<T>& polygon) {
    if (polygon.size() < 3) return 0;
    auto it = polygon.begin();
    return ccw(policy, it[0], it[1], it[2]);
}

}  // namespace ac
