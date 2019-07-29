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

template <class Policy, class T, enable_int_if_policy<Policy> = 0>
inline int ccw(const Policy& policy, const convex_polygon<T>& poly) {
    if (poly.size() < 3) return 0;
    auto it = poly.begin();
    return ccw(policy, it[0], it[1], it[2]);
}

}  // namespace ac
