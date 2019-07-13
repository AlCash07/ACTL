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
#include <actl/geometry/algorithm/project/line.hpp>
#include <actl/geometry/detail/between_endpoints.hpp>
#include <actl/geometry/polygon.hpp>

namespace ac {

template <class P = use_default, class AreaPolicy = comparable_area_points<P>>
struct intersect_line_simple_polygon : intersect_line_line<AreaPolicy> {
    ccw_policy<AreaPolicy> ccw_policy;
};

namespace detail {

/**
 * Line with simple polygon intersection : O(N).
 */
template <class P, class AP, class T0, class K, class T1, class It>
inline auto intersect(const intersect_line_simple_polygon<P, AP>& policy,
                      const line<T0, 2, K>& line, const simple_polygon<T1>& polygon, It dst) {
    using O = geometry::scalar_t<output_type_t<It>>;
    project_line_scalar<P, O> project_policy;
    ACTL_ASSERT(line);
    auto vertex_sgn = [&](auto it) { return ccw(policy.ccw_policy, *it, line); };
    auto i = polygon.cyclic_begin();
    int prev_sgn = vertex_sgn(i - 1), i_sgn = vertex_sgn(i), next_sgn;
    for (index n = polygon.size(); n --> 0; prev_sgn = i_sgn, i_sgn = next_sgn) {
        next_sgn = vertex_sgn(i + 1);
        if (i_sgn == 0) {
            bool ok = false;
            if (prev_sgn == 0) {
                ok = next_sgn == sgn(dot<P>(i[0] - i[-1], line.vector));
            } else if (next_sgn == 0) {
                ok = prev_sgn == sgn(dot<P>(i[1] - i[0], line.vector));
            } else {
                ok = next_sgn != prev_sgn;
            }
            if (ok && between_endpoints<P>(*i, line))
                *dst++ = project(project_policy, *i, line);
        } else if (next_sgn == -i_sgn) {
            dst = intersect(policy, line, make_line(i[0], i[1]), dst);
        }
    }
}

}  // namespace detail

template <class P, class AP, class T0, class K, class T1, class OutIter>
inline auto intersect(const intersect_line_simple_polygon<P, AP>& policy,
                      const line<T0, 2, K>& line, const simple_polygon<T1>& polygon, OutIter dst) {
    return detail::intersect(policy, line, polygon, detail::adapt_iterator(line, dst));
}

template <class T0, class K, class T1, class OutIter>
inline auto intersect(use_default, const line<T0, 2, K>& line, const simple_polygon<T1>& polygon,
                      OutIter dst) {
    return intersect(intersect_line_simple_polygon{}, line, polygon, dst);
}

}  // namespace ac
