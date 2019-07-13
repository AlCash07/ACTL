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

namespace ac {

template <class CcwPolicy = comparable_ccw<>>
struct within_convex_monotone_polygon : CcwPolicy {};

/**
 * O(log N).
 */
template <class CP, class T0, class T1>
inline int within(const within_convex_monotone_polygon<CP>& policy, const point<T0>& point,
                  const convex_monotone_polygon<T1>& polygon) {
    if (polygon.empty()) return 0;
    if (polygon.size() == 1) return point == polygon[0];
    const int right = polygon.right();
    if (point < polygon[0] || point > polygon[right]) return 0;
    switch (ccw(policy, point, polygon[right], polygon[0])) {
        case 0: {
            if (point == polygon[0] || point == polygon[right]) return 1;
            return right == 1 || right + 1 == polygon.size() ? 1 : 2;
        }
        case -1: {  // lower chain
            auto lit = std::lower_bound(polygon.begin() + 1, polygon.begin() + right, point);
            return 1 - ccw(policy, lit[0], point, lit[-1]);
        }
        default: {  // upper chain
            auto uit = std::lower_bound(polygon.rbegin(), polygon.rend() - right - 1, point);
            return 1 - ccw(policy, uit == polygon.rbegin() ? polygon[0] : uit[-1], point, uit[0]);
        }
    }
}

template <class T0, class T1>
inline int within(use_default, const point<T0>& point, const convex_monotone_polygon<T1>& polygon) {
    return within(within_convex_monotone_polygon{}, point, polygon);
}

}  // namespace ac
