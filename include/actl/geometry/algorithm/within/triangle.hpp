// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/geometry/algorithm/orientation/point_point.hpp>
#include <actl/geometry/algorithm/within/within.hpp>
#include <actl/geometry/polygon.hpp>

namespace ac {

/// Point vs non-degenerate triangle : O(1).
/// Joseph O'Rourke, Computational Geometry in C (2nd Edition), p. 235.
template <class Policy, class T, class U>
enum within within(
    Policy const& policy, point<T> const& p, polygon<U> const& triangle)
{
    ACTL_ASSERT(triangle.size() == 3);
    orientation2d signs[3] = {};
    auto it = cyclic_begin(triangle);
    for (index i = 0; i < 3; ++i, ++it)
    {
        signs[i] = orientation(policy, p, it[1], it[0]);
    }
    if (signs[0] == signs[1] && signs[1] == signs[2])
        return within::inside;
    for (index i = 0; i < 3; ++i)
    {
        if (signs[i] != orientation2d::collinear &&
            signs[i] == -signs[i + 1 == 3 ? 0 : i + 1])
            return within::outside;
    }
    return within::border;
}

} // namespace ac
