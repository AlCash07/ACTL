/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/ccw/point_point.hpp>
#include <actl/geometry/polygon.hpp>
#include <algorithm>
#include <limits>

namespace ac {

/**
 * Winding number - number of turns around the point in counter-clockwise direction.
 * If the point is on the boundary, returns std::numeric_limits<int>::max().
 * http://geomalgorithms.com/a03-_inclusion.html
 */
template <class Policy, class T, class U>
inline int winding_number(const Policy& policy, const point<T>& p, const polygon<U>& poly) {
    static constexpr int boundary = std::numeric_limits<int>::max();
    if (poly.empty()) return 0;
    index n = static_cast<index>(poly.size());
    if (n == 1) return equal(policy, p, poly[0]) ? boundary : 0;
    int res = 0;
    for (auto i = cyclic_begin(poly); n --> 0; ++i) {
        if (equal(policy, p, *i)) return boundary;
        auto j = i + 1;
        if (equal(policy, i->y(), p.y()) && equal(policy, j->y(), p.y())) {
            if (less(policy, i->x(), j->x())) {
                if (!less(policy, p.x(), i->x()) && !less(policy, j->x(), p.x())) return boundary;
            } else {
                if (!less(policy, p.x(), j->x()) && !less(policy, i->x(), p.x())) return boundary;
            }
        } else {
            bool below = less(policy, i->y(), p.y());
            if (below != less(policy, j->y(), p.y())) {
                int orientation = ccw(policy, p, *j, *i);
                if (orientation == 0) return boundary;
                if (below == (orientation > 0))
                    res += below ? 1 : -1;
            }
        }
    }
    return res;
}

}  // namespace ac
