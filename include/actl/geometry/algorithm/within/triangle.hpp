/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/geometry/algorithm/ccw/point_point.hpp>
#include <actl/geometry/polygon.hpp>

namespace ac {

template <class CcwPolicy = comparable_ccw<>>
struct within_triangle : CcwPolicy {};

/**
 * Point vs non-degenerate triangle : O(1).
 * Joseph O'Rourke, Computational Geometry in C (2nd Edition), p. 235.
 */
template <class CP, class T0, class T1>
inline int within(const within_triangle<CP>& policy, const point<T0>& point,
                  const polygon<T1>& triangle) {
    ACTL_ASSERT(triangle.size() == 3);
    int signs[3] = {};
    auto it = triangle.cyclic_begin();
    for (index i = 0; i < 3; ++i, ++it) {
        signs[i] = ccw(policy, point, it[1], it[0]);
    }
    if (signs[0] == signs[1] && signs[1] == signs[2]) return 2;
    for (index i = 0; i < 3; ++i) {
        if (signs[i] * signs[i + 1 == 3 ? 0 : i + 1] == -1) return 0;
    }
    return 1;
}

}  // namespace acr
