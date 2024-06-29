// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/orientation/point_point.hpp>
#include <actl/geometry/polygon.hpp>

namespace ac {

template<class T>
orientation2d orientation(
    Policy auto const& policy, convex_polygon<T> const& poly
) {
    if (poly.size() < 3)
        return orientation2d::collinear;
    auto it = poly.begin();
    return orientation(policy, it[0], it[1], it[2]);
}

} // namespace ac
