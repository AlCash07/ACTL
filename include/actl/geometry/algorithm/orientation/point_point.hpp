// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>
#include <actl/geometry/algorithm/orientation/orientation.hpp>

namespace ac {

template<typename TL, typename TR>
constexpr orientation2d orientation(
    Policy auto const& policy, point<TL> const& l, point<TR> const& r
) {
    return static_cast<orientation2d>(sgn(policy, area(policy, r, l)));
}

template<typename TL, typename TR, typename TO>
constexpr orientation2d orientation(
    Policy auto const& policy,
    point<TL> const& l,
    point<TR> const& r,
    point<TO> const& origin
) {
    return orientation(policy, l - origin, r - origin);
}

template<index N, typename TL, typename TR>
enum orientation orientation(
    Policy auto const& policy, point<TL, N> const& l, point<TR, N> const& r
) {
    index i = 0;
    for (; i < N && equal(policy, l[i], 0); ++i) {
        if (!equal(policy, r[i], 0))
            return orientation::non_collinear;
    }
    for (index j = i + 1; j < N; ++j) {
        if (!collinear(policy, point{l[i], l[j]}, point{r[i], r[j]}))
            return orientation::non_collinear;
    }
    return orientation::collinear;
}

} // namespace ac
