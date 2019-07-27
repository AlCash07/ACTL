/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/sqrt.hpp>
#include <actl/geometry/2d/point2d.hpp>
#include <actl/geometry/3d/point3d.hpp>
#include <actl/geometry/algorithm/area/area.hpp>

namespace ac {

template <class P = use_default>
struct comparable_area_points : geometry::policy {};

template <class P = use_default, class F = use_default>
struct standard_area_points : geometry::policy {};

/**
 * Oriented area of parallelogram with side vectors being p0 and p1.
 * 2D: positive if p1 <- p0 counter-clockwise.
 */
// TODO: figure out the correct sign instead of returning the absolute value for dimension > 2.
template <class Policy, index N, class T0, class T1>
inline auto area(Policy&& policy, const point<T0, N>& p0, const point<T1, N>& p1) {
    if constexpr (N == 2) {
        return product(policy, p0[0], p1[1]) - product(policy, p0[1], p1[0]);
    } else if constexpr (N == 3) {
        return sqrt(policy, dot(policy, cross(policy, p0, p1)));
    } else {
        auto a = product(policy, dot(policy, p0), dot(policy, p1));
        return sqrt(policy, a - sqr(policy, dot(policy, p0, p1)));
    }
}

}  // namespace ac
