// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/geometry/algorithm/orientation/point_point.hpp>
#include <actl/geometry/detail/to_inclusion.hpp>
#include <actl/geometry/polygon.hpp>
#include <algorithm>

namespace ac {

/**
 * O(log N).
 */
template <class Policy, class T, class U>
enum within within(const Policy& policy, const point<T>& p, const monotone_polygon<U>& poly) {
    if (poly.empty()) return within::outside;
    if (poly.size() == 1) return equal(policy, p, poly[0]) ? within::border : within::outside;
    const index right = poly.right();
    auto first = poly.begin();
    if (less(policy, p, poly[0]) || less(policy, first[right], p)) return within::outside;
    auto lit = std::lower_bound(first + 1, first + right, p, math::less(policy));
    auto orient = orientation(policy, p, lit[0], lit[-1]);
    if (orient == orientation2d::right) {
        auto uit = std::lower_bound(poly.rbegin(), poly.rend() - right - 1, p, math::less(policy));
        orient = orientation(policy, p, uit == poly.rbegin() ? poly[0] : uit[-1], uit[0]);
    }
    return detail::to_inclusion(orient);
}

}  // namespace ac
