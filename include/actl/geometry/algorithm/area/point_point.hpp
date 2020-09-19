/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/scalar/math.hpp>
#include <actl/geometry/algorithm/area/area.hpp>
#include <actl/geometry/point.hpp>

namespace ac {

/**
 * Oriented area of parallelogram with side vectors being p0 and p1.
 * 2D: positive if p1 <- p0 counter-clockwise.
 */
template <class Policy, index N, class T0, class T1>
inline auto area(const Policy& policy, const point<T0, N>& p0, const point<T1, N>& p1) {
    if constexpr (N == 2) {
        return product(policy, p0[0], p1[1]) - product(policy, p0[1], p1[0]);
    } else if constexpr (N == 3) {
        // TODO: figure out the correct sign instead of returning the absolute value.
        return sqrt(policy, dot(policy, cross(policy, p0, p1)));
    } else {
        auto a = product(policy, dot(policy, p0), dot(policy, p1));
        return sqrt(policy, a - sqr(policy, dot(policy, p0, p1)));
    }
}

}  // namespace ac
