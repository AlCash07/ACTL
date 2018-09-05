/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/ccw/point_point.hpp>
#include <actl/geometry/polygon.hpp>
#include <actl/range/algorithm.hpp>

namespace ac {

/**
 * Orientation of a simple polygon : fast O(N).
 * Joseph O'Rourke, Computational Geometry in C (2nd Edition), p. 12.
 */
template <class AP, class T>
inline int ccw(const ccw_policy<AP>& policy, const simple_polygon<T>& polygon) {
    auto it = polygon.cyclic(min_element(polygon));
    return ccw(policy, it[-1], it[0], it[1]);
}

}  // namespace ac
