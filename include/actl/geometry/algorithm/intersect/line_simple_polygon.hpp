// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/assert.hpp>
#include <actl/geometry/algorithm/intersect/intersect.hpp>
#include <actl/geometry/algorithm/intersect/line_line_2d.hpp>
#include <actl/geometry/algorithm/orientation/point_line.hpp>
#include <actl/geometry/algorithm/project/point_line.hpp>
#include <actl/geometry/detail/between_endpoints.hpp>
#include <actl/geometry/polygon.hpp>

namespace ac {

/**
 * Line with simple polygon intersection : O(N).
 */
template <class Policy, class T, class K, class U, class OutIter>
OutIter intersect(line_scalar_policy<Policy> lsp, const line<T, 2, K>& l,
                  const simple_polygon<U>& poly, OutIter dst) {
    auto& policy = lsp.policy;
    // TODO: fix the case when polygon touches the line.
    ACTL_ASSERT(!degenerate(policy, l));
    auto vertex_sgn = [&](auto iter) { return static_cast<int>(orientation(policy, *iter, l)); };
    auto iter = cyclic_begin(poly);
    int prev_sgn = vertex_sgn(it - 1), iter_sgn = vertex_sgn(it);
    for (auto n = poly.size(); n != 0; --n) {
        int next_sgn = vertex_sgn(iter + 1);
        if (iter_sgn == 0) {
            bool ok = false;
            if (prev_sgn == 0)
                ok = next_sgn == sgn(policy, dot(policy, *iter - iter[-1], l.vector));
            else if (next_sgn == 0)
                ok = prev_sgn == sgn(policy, dot(policy, iter[1] - *iter, l.vector));
            else
                ok = next_sgn != prev_sgn;
            if (ok && between_endpoints(policy, *iter, l))
                *dst++ = project(lsp, *iter, l);
        } else if (next_sgn == -iter_sgn) {
            dst = intersect(lsp, l, make_line(*iter, iter[1]), dst);
        }
        prev_sgn = iter_sgn;
        iter_sgn = next_sgn;
        ++iter;
    }
    return dst;
}

}  // namespace ac
