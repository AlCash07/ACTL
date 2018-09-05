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

template <class CcwPolicy = comparable_ccw<>>
struct winding_number_policy : CcwPolicy {};

/**
 * Winding number - number of turns around the point in counter-clockwise direction.
 * If the point is on the boundary, returns std::numeric_limits<int>::max().
 * http://geomalgorithms.com/a03-_inclusion.html
 */
template <class CP, class T0, class T1>
inline int winding_number(const winding_number_policy<CP>& policy, const point<T0>& point,
                          const polygon<T1>& polygon) {
    static constexpr int boundary = std::numeric_limits<int>::max();
    if (polygon.empty()) return 0;
    int n = polygon.size();
    if (n == 1) return point == polygon[0] ? boundary : 0;
    int res = 0;
    for (auto i = polygon.cyclic_begin(); n --> 0; ++i) {
        if (point == *i) return boundary;
        auto j = i + 1;
        if (i->y() == point.y() && j->y() == point.y()) {
            if (std::min(i->x(), j->x()) <= point.x() && point.x() <= std::max(i->x(), j->x()))
                return boundary;
        } else {
            bool below = i->y() < point.y();
            if (below != (j->y() < point.y())) {
                int orientation = ccw(policy, point, *j, *i);
                if (orientation == 0) return boundary;
                if (below == (orientation > 0))
                    res += below ? 1 : -1;
            }
        }
    }
    return res;
}

template <class T0, class T1>
inline int winding_number(const point<T0>& point, const polygon<T1>& polygon) {
    return winding_number(winding_number_policy<>(), point, polygon);
}

}  // namespace ac
