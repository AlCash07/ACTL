/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/ccw/point_point.hpp>
#include <actl/geometry/polygon.hpp>

namespace ac {

template <class CcwPolicy = comparable_ccw<>>
struct extreme_vertex_policy : CcwPolicy {};

/**
 * Extreme (minimum ccw) point index with respect to the given direction : O(log N).
 * Joseph O'Rourke, Computational Geometry in C (2nd Edition), p. 270.
 */
template <class CP, class T, class Function>
inline auto extreme_vertex(const extreme_vertex_policy<CP>& policy,
                           const convex_polygon<T>& polygon, Function direction) {
    auto vertex_cmp = [&](auto i, auto j) {  // < 0 if i is before j
        return ccw(policy, direction(*j), *j - *i);
    };
    auto is_extreme = [vertex_cmp](auto i, int& i_sgn) {
        return (i_sgn = vertex_cmp(i + 1, i)) >= 0 && vertex_cmp(i, i - 1) < 0;
    };
    auto first = polygon.begin();
    int first_sgn{}, middle_sgn{};
    if (is_extreme(polygon.cyclic(first), first_sgn)) return first;
    for (auto last = polygon.end(); first + 1 != last;) {
        auto middle = first + (last - first) / 2;
        if (is_extreme(polygon.cyclic(middle), middle_sgn)) return middle;
        if (first_sgn != middle_sgn ? first_sgn < middle_sgn
                                    : first_sgn == vertex_cmp(first, middle)) {
            last = middle;
        } else {
            first     = middle;
            first_sgn = middle_sgn;
        }
    }
    return first;
}

template <class T, class Function>
inline auto extreme_vertex(const convex_polygon<T>& polygon, Function direction) {
    return extreme_vertex(extreme_vertex_policy<>{}, polygon, direction);
}

}  // namespace ac
