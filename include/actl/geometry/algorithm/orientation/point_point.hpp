// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>
#include <actl/geometry/algorithm/orientation/orientation.hpp>

namespace ac {

template<class T0, class T1>
constexpr orientation2d orientation(
    Policy auto const& policy, point<T0> const& lhs, point<T1> const& rhs
) {
    return static_cast<orientation2d>(sgn(policy, area(policy, rhs, lhs)));
}

template<class T0, class T1, class T2>
constexpr orientation2d orientation(
    Policy auto const& policy,
    point<T0> const& lhs,
    point<T1> const& rhs,
    point<T2> const& origin
) {
    return orientation(policy, lhs - origin, rhs - origin);
}

template<index N, class T0, class T1>
enum orientation orientation(
    Policy auto const& policy, point<T0, N> const& lhs, point<T1, N> const& rhs
) {
    index i = 0;
    for (; i < N && equal(policy, lhs[i], 0); ++i) {
        if (!equal(policy, rhs[i], 0))
            return orientation::non_collinear;
    }
    for (index j = i + 1; j < N; ++j) {
        if (!collinear(policy, point{lhs[i], lhs[j]}, point{rhs[i], rhs[j]}))
            return orientation::non_collinear;
    }
    return orientation::collinear;
}

} // namespace ac
