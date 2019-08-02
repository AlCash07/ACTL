/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/geometry/algorithm/ccw/point_line.hpp>
#include <actl/geometry/algorithm/intersect/intersect.hpp>
#include <actl/geometry/algorithm/intersect/line_line_2d.hpp>
#include <actl/geometry/algorithm/project/line.hpp>
#include <actl/geometry/detail/between_endpoints.hpp>
#include <actl/geometry/polygon.hpp>

namespace ac {

namespace detail {

/**
 * Line with simple polygon intersection : O(N).
 */
template <class Policy, class T, class K, class U, class OutIter>
inline OutIter intersect_lsp(const Policy& policy, const line<T, 2, K>& l,
                             const simple_polygon<U>& poly, OutIter dst) {
    // TODO: fix the case when polygon touches the line.
    ACTL_ASSERT(!degenerate(policy, l));
    auto vertex_sgn = [&](auto it) { return ccw(policy, *it, l); };
    auto it = cyclic_begin(poly);
    int prev_sgn = vertex_sgn(it - 1), it_sgn = vertex_sgn(it);
    for (auto n = poly.size(); n != 0; --n) {
        int next_sgn = vertex_sgn(it + 1);
        if (it_sgn == 0) {
            bool ok = false;
            if (prev_sgn == 0) {
                ok = next_sgn == sgn(policy, dot(policy, *it - it[-1], l.vector));
            } else if (next_sgn == 0) {
                ok = prev_sgn == sgn(policy, dot(policy, it[1] - *it, l.vector));
            } else {
                ok = next_sgn != prev_sgn;
            }
            if (ok && between_endpoints(policy, *it, l))
                *dst++ = project(line_scalar_policy{policy}, *it, l);
        } else if (next_sgn == -it_sgn) {
            dst = intersect(line_scalar_policy{policy}, l, make_line(*it, it[1]), dst);
        }
        prev_sgn = it_sgn;
        it_sgn = next_sgn;
        ++it;
    }
    return dst;
}

}  // namespace detail

template <class Policy, class T, class K, class U, class OutIter>
inline OutIter intersect(const line_scalar_policy<Policy>& lcp, const line<T, 2, K>& l,
                         const simple_polygon<U>& poly, OutIter dst) {
    return detail::intersect_lsp(lcp.policy, l, poly, dst);
}

template <class Policy, class T, class K, class U, class OutIter>
inline OutIter intersect(const Policy& policy, const line<T, 2, K>& l,
                         const simple_polygon<U>& poly, OutIter dst) {
    return detail::intersect_lsp(policy, l, poly, detail::scalar_to_point_adaptor{l, dst});
}

}  // namespace ac
