// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/geometry/algorithm/area/point_line.hpp>
#include <actl/geometry/algorithm/intersect/intersect.hpp>
#include <actl/geometry/detail/line_test.hpp>
#include <actl/geometry/sphere.hpp>

namespace ac {

template <class Policy, index N, class T0, class K, class T1, class OutIter>
OutIter intersect(line_scalar_policy<Policy> lsp, const line<T0, N, K>& l, const sphere<T1, N>& s,
                  OutIter dst) {
    auto& policy = lsp.policy;
    auto vdot = dot(policy, l.vector);
    auto delta =
        product(policy, vdot, sqr(policy, s.radius)) - sqr(policy, area(policy, s.center, l));
    int delta_sgn = sgn(policy, delta);
    if (delta_sgn < 0)
        return dst;
    auto projection = dot(policy, s.center - l.begin, l.vector);
    auto output = [&](const auto& x) {
        if (detail::line_test(policy, l.kind(), x, vdot)) {
            *dst++ = ratio(policy, x, vdot);
        }
    };
    if (delta_sgn == 0) {
        output(projection);
    } else {
        auto offset = sqrt(policy, delta);
        output(projection - offset);
        output(projection + offset);
    }
    return dst;
}

}  // namespace ac
