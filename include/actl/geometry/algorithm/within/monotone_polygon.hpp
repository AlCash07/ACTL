/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/ccw/point_point.hpp>
#include <actl/geometry/algorithm/within/within.hpp>
#include <actl/geometry/polygon.hpp>
#include <algorithm>

namespace ac {

/**
 * O(log N).
 */
template <class Policy, class T, class U>
inline int within(const Policy& policy, const point<T>& p, const monotone_polygon<U>& poly) {
    if (poly.empty()) return 0;
    if (poly.size() == 1) return equal(policy, p, poly[0]);
    const index right = poly.right();
    auto first = poly.begin();
    if (less(policy, p, poly[0]) || less(policy, first[right], p)) return 0;
    auto lit = std::lower_bound(first + 1, first + right, p, op::less_functor(policy));
    int orientation = ccw(policy, lit[0], p, lit[-1]);
    if (orientation >= 0) return 1 - orientation;
    auto uit =
        std::lower_bound(poly.rbegin(), poly.rend() - right - 1, p, op::less_functor(policy));
    return 1 - ccw(policy, uit == poly.rbegin() ? poly[0] : uit[-1], p, uit[0]);
}

}  // namespace ac
