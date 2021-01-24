// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>
#include <actl/geometry/algorithm/orientation/orientation.hpp>

namespace ac {

template <class Policy, class T0, class T1, enable_int_if_policy<Policy> = 0>
constexpr orientation2d orientation(
    const Policy& policy, const point<T0>& lhs, const point<T1>& rhs) //
{
    return static_cast<orientation2d>(sgn(policy, area(policy, rhs, lhs)));
}

template <
    class Policy,
    class T0,
    class T1,
    class T2,
    enable_int_if_policy<Policy> = 0>
constexpr orientation2d orientation(
    const Policy& policy,
    const point<T0>& lhs,
    const point<T1>& rhs,
    const point<T2>& origin) //
{
    return orientation(policy, lhs - origin, rhs - origin);
}

template <
    class Policy,
    index N,
    class T0,
    class T1,
    enable_int_if_policy<Policy> = 0>
enum orientation orientation(
    const Policy& policy, const point<T0, N>& lhs, const point<T1, N>& rhs) //
{
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
