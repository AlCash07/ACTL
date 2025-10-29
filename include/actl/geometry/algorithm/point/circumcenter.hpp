// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/intersect/line_line_2d.hpp>
#include <actl/geometry/algorithm/orientation/point_point.hpp>

namespace ac {

template<typename T0, typename T1, typename T2>
auto circumcenter(
    Policy auto const& policy,
    point<T0> const& a,
    point<T1> const& b,
    point<T2> const& c
) {
    AC_ASSERT(!collinear(policy, a, b, c));
    using T = geometry::scalar_t<T0, T1, T2>;
    // TODO: use actual return type of intersect.
    point<decltype(ratio(policy, T{}, T{}))> res;
    intersect(
        general_position_policy{policy},
        make_line(a + b, perpendicular(a - b), true),
        make_line(a + c, perpendicular(a - c), true),
        &res
    );
    return ratio(policy, res, 2);
}

template<typename T0, typename T1, typename T2>
auto circumcenter(point<T0> const& a, point<T1> const& b, point<T2> const& c) {
    return circumcenter(geometry_policy, a, b, c);
}

} // namespace ac
