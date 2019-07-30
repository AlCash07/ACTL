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
template <class Policy, class T0, class T1>
inline int within(const Policy& policy, const point<T0>& p,
                  const convex_monotone_polygon<T1>& poly) {
    if (poly.empty()) return 0;
    if (poly.size() == 1) return equal(policy, p, poly[0]);
    const index right = poly.right();
    auto first = poly.begin();
    if (less(policy, p, poly[0]) || less(policy, first[right], p)) return 0;
    switch (ccw(policy, p, first[right], poly[0])) {
        case 0: {
            if (equal(policy, p, poly[0]) || equal(policy, p, first[right])) return 1;
            return right == 1 || right + 1 == poly.size() ? 1 : 2;
        }
        case -1: {  // lower chain
            auto lit = std::lower_bound(first + 1, first + right, p, op::less_functor(policy));
            return 1 - ccw(policy, lit[0], p, lit[-1]);
        }
        case 1: {  // upper chain
            auto uit = std::lower_bound(poly.rbegin(), poly.rend() - right - 1, p,
                                        op::less_functor(policy));
            return 1 - ccw(policy, uit == poly.rbegin() ? poly[0] : uit[-1], p, uit[0]);
        }
    }
    ACTL_ASSERT(false);
    return 0;
}

}  // namespace ac
