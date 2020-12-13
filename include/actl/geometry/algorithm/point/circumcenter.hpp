// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/geometry/algorithm/intersect/line_line_2d.hpp>
#include <actl/geometry/algorithm/orientation/point_point.hpp>

namespace ac {

template <class Policy, class T0, class T1, class T2>
inline auto circumcenter(const Policy& policy, const point<T0>& a, const point<T1>& b,
                         const point<T2>& c) {
    ACTL_ASSERT(!collinear(policy, a, b, c));
    using T = geometry::scalar_t<T0, T1, T2>;
    // TODO: use actual return type of intersect.
    point<decltype(ratio(policy, T{}, T{}))> res;
    intersect(general_position_policy{policy}, make_line(a + b, perpendicular(a - b), true),
              make_line(a + c, perpendicular(a - c), true), &res);
    return ratio(policy, res, 2);
}

template <class T0, class T1, class T2>
inline auto circumcenter(const point<T0>& a, const point<T1>& b, const point<T2>& c) {
    return circumcenter(geometry_policy, a, b, c);
}

}  // namespace ac
