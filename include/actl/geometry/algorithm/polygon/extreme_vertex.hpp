/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/ccw/point_point.hpp>
#include <actl/geometry/polygon.hpp>
#include <actl/std/utility.hpp>

namespace ac {

/**
 * Extreme (minimum ccw) point index with respect to the given direction : O(log N).
 * If there are two extreme vertices, the first one in counter-clockwise order is taken.
 * Joseph O'Rourke, Computational Geometry in C (2nd Edition), p. 270.
 */
template <class Policy, class T, class Function>
inline auto extreme_vertex(const Policy& policy, const convex_polygon<T>& poly,
                           Function direction) {
    auto vertex_cmp = [&](auto i, auto j) {  // < 0 if i is before j
        return ccw(policy, direction(*j), *j - *i);
    };
    auto is_extreme = [&](auto it) {
        auto i = cyclic_iterator{poly, it};
        int v = vertex_cmp(i + 1, i);
        return std::pair{0 <= v && vertex_cmp(i, i - 1) < 0, v};
    };
    auto first = poly.begin();
    auto [first_ok, first_sgn] = is_extreme(first);
    if (first_ok) return first;
    for (auto last = poly.end(); first + 1 != last;) {
        auto middle = first + (last - first) / 2;
        auto [middle_ok, middle_sgn] = is_extreme(middle);
        if (middle_ok) return middle;
        if (first_sgn != middle_sgn ? first_sgn < middle_sgn
                                    : first_sgn == vertex_cmp(first, middle)) {
            last = middle;
        } else {
            first = middle;
            first_sgn = middle_sgn;
        }
    }
    return first;
}

template <class T, class Function>
inline auto extreme_vertex(const convex_polygon<T>& poly, Function direction) {
    return extreme_vertex(geometry_policy, poly, direction);
}

}  // namespace ac
