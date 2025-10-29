// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/orientation/point_point.hpp>
#include <actl/geometry/polygon.hpp>
#include <actl/range/algorithm.hpp>

namespace ac {

/// Orientation of a simple polygon : fast O(N).
/// Joseph O'Rourke, Computational Geometry in C (2nd Edition), p. 12.
template<typename T>
orientation2d orientation(
    Policy auto const& policy, simple_polygon<T> const& poly
) {
    auto it = cyclic_iterator{poly, min_element(poly, less(policy))};
    return orientation(policy, it[-1], it[0], it[1]);
}

} // namespace ac
