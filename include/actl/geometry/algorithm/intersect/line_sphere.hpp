// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/area/point_line.hpp>
#include <actl/geometry/algorithm/intersect/intersect.hpp>
#include <actl/geometry/detail/line_test.hpp>
#include <actl/geometry/sphere.hpp>

namespace ac {

template<
    typename P,
    index N,
    typename T0,
    typename K,
    typename T1,
    typename OutIter>
OutIter intersect(
    line_scalar_policy<P> lsp,
    line<T0, N, K> const& l,
    sphere<T1, N> const& s,
    OutIter output
) {
    auto& policy = lsp.policy;
    auto vdot = dot(policy, l.vector);
    auto delta = product(policy, vdot, squared(policy, s.radius)) -
                 squared(policy, area(policy, s.center, l));
    int delta_sgn = sgn(policy, delta);
    if (delta_sgn < 0)
        return output;
    auto projection = dot(policy, s.center - l.begin, l.vector);
    auto check = [&](auto const& x) {
        if (detail::line_test(policy, l.kind(), x, vdot)) {
            *output++ = ratio(policy, x, vdot);
        }
    };
    if (delta_sgn == 0) {
        check(projection);
    } else {
        auto offset = square_root(policy, delta);
        check(projection - offset);
        check(projection + offset);
    }
    return output;
}

} // namespace ac
