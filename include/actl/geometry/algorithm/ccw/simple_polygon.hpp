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
template <class Policy, class T, enable_int_if_policy<Policy> = 0>
inline int ccw(const Policy& policy, const simple_polygon<T>& poly) {
    auto it = cyclic_iterator{poly, min_element(poly, op::less_functor(policy))};
    return ccw(policy, it[-1], it[0], it[1]);
}

}  // namespace ac
