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
template<index N, typename T>
auto angle(Policy auto const& policy, point<T, N> const& p) {
    return atan2(p[1], p[0]);
}

template<index N, typename TL, typename TR>
auto angle(
    Policy auto const& policy, point<TL, N> const& l, point<TR, N> const& r
) {
    return atan2(area(policy, l, r), dot(policy, l, r));
}

template<index N, typename TL, typename TR, typename TO>
auto angle(
    Policy auto const& policy,
    point<TL, N> const& l,
    point<TR, N> const& r,
    point<TO, N> const& origin
) {
    return angle(policy, l - origin, r - origin);
}

template<typename T, typename... Ts, disable_int_if_policy<T> = 0>
auto angle(T const& x, Ts const&... xs) {
    return angle(geometry_policy, x, xs...);
}

} // namespace ac
