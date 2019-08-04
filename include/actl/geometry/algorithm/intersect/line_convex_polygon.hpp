/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/geometry/algorithm/ccw/point_line.hpp>
#include <actl/geometry/algorithm/intersect/line_line_2d.hpp>
#include <actl/geometry/algorithm/polygon/extreme_vertex.hpp>

namespace ac {

/**
 * Intersection of line with convex polygon: O(log N).
 * Joseph O'Rourke, Computational Geometry in C (2nd Edition), p. 271.
 */
template <class Policy, class T, class K, class U, class OutIter>
inline OutIter intersect(const line_scalar_policy<Policy>& lsp, const line<T, 2, K>& l,
                         const convex_polygon<U>& poly, OutIter dst) {
    auto& policy = lsp.policy;
    ACTL_ASSERT(!degenerate(policy, l));
    auto right =
        cyclic_iterator{poly, extreme_vertex(policy, poly, [&l](const auto&) { return l.vector; })};
    auto left = cyclic_iterator{
        poly, extreme_vertex(policy, poly, [&l](const auto&) { return -l.vector; })};
    auto vertex_sgn = [&](auto it) { return ccw(policy, *it, l); };
    int right_sgn = vertex_sgn(right);
    int left_sgn = vertex_sgn(left);
    if (left_sgn < 0 || 0 < right_sgn) return dst;
    // TODO: in case line passes through exactly one vertex, it's reported twice. Fix this.
    auto intersect_chain = [&](auto first, auto last, int first_sgn, OutIter dst) {
        while (first + 1 != last) {
            auto middle = first + (last - first) / 2;
            if (vertex_sgn(middle) == first_sgn) {
                first = middle;
            } else {
                last = middle;
            }
        }
        return intersect(line_scalar_policy{policy}, l, make_line(*first, *last), dst);
    };
    dst = intersect_chain(left, right, left_sgn, dst);
    return intersect_chain(right, left, right_sgn, dst);
}

}  // namespace ac
