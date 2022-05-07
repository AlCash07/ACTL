// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>
#include <actl/numeric/math.hpp>

namespace ac {

/// Counter-clockwise angle.
template <class Policy, index N, class T, enable_int_if_policy<Policy> = 0>
auto angle(Policy const& policy, point<T, N> const& p)
{
    return atan2(p[1], p[0]);
}

template <
    class Policy,
    index N,
    class T0,
    class T1,
    enable_int_if_policy<Policy> = 0>
auto angle(
    Policy const& policy, point<T0, N> const& lhs, point<T1, N> const& rhs)
{
    return atan2(area(policy, lhs, rhs), dot(policy, lhs, rhs));
}

template <
    class Policy,
    index N,
    class T0,
    class T1,
    class T2,
    enable_int_if_policy<Policy> = 0>
auto angle(
    Policy const& policy,
    point<T0, N> const& lhs,
    point<T1, N> const& rhs,
    point<T2, N> const& origin)
{
    return angle(policy, lhs - origin, rhs - origin);
}

template <class T, class... Ts, disable_int_if_policy<T> = 0>
auto angle(T const& x, Ts const&... xs)
{
    return angle(geometry_policy, x, xs...);
}

} // namespace ac
