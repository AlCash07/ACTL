/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/geometry/algorithm/intersect/line_line_2d.hpp>
#include <actl/geometry/algorithm/polygon/extreme_vertex.hpp>

namespace ac {

template <class AreaPolicy = comparable_area_points<>>
struct intersect_line_convex_polygon : extreme_vertex_policy<ccw_policy<AreaPolicy>> {
    intersect_line_line<AreaPolicy> intersect_policy;
};

namespace detail {

/**
 * Line with convex polygon intersection: O(log N).
 * Joseph O'Rourke, Computational Geometry in C (2nd Edition), p. 271.
 */
template <class AP, class T0, class K, class T1, class It>
inline auto intersect(const intersect_line_convex_polygon<AP>& policy, const line<T0, 2, K>& line,
                      const convex_polygon<T1>& polygon, It dst) {
    ACTL_ASSERT(line);
    auto right = polygon.cyclic(
        extreme_vertex(policy, polygon, [&line](const point<T1>&) { return line.slope; }));
    auto left = polygon.cyclic(
        extreme_vertex(policy, polygon, [&line](const point<T1>&) { return -line.slope; }));
    auto vertex_sgn = [&](auto it) { return ccw(policy, *it, line); };
    int right_sgn = vertex_sgn(right), left_sgn = vertex_sgn(left);
    if (left_sgn < 0 || 0 < right_sgn) return dst;
    auto intersect_chain = [&](auto first, auto last, int first_sgn, It dst) {
        while (first + 1 != last) {
            auto middle = first + (last - first) / 2;
            if (vertex_sgn(middle) == first_sgn) {
                first = middle;
            } else {
                last = middle;
            }
        }
        return intersect(policy.intersect_policy, line, make_line(*first, *last), dst);
    };
    dst = intersect_chain(left, right, left_sgn, dst);
    return intersect_chain(right, left, right_sgn, dst);
}

}  // namespace detail

template <class AP, class T0, class K, class T1, class OutputIterator>
inline auto intersect(const intersect_line_convex_polygon<AP>& policy, const line<T0, 2, K>& line,
                      const convex_polygon<T1>& polygon, OutputIterator dst) {
    return detail::intersect(policy, line, polygon, detail::adapt_iterator(line, dst));
}

template <class T0, class K, class T1, class OutputIterator>
inline auto intersect(use_default, const line<T0, 2, K>& line, const convex_polygon<T1>& polygon,
                      OutputIterator dst) {
    return intersect(intersect_line_convex_polygon<>(), line, polygon, dst);
}

}  // namespace ac
