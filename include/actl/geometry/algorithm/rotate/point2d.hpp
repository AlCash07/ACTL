// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/rotate/rotate.hpp>
#include <actl/numeric/math.hpp>

namespace ac {

template <class T0, class T1>
auto rotate(Policy auto const& policy, point<T0> const& p, T1 const& angle) {
    auto sin = sin(angle);
    auto cos = cos(angle);
    return point{
        dot(policy, point{cos, sin}, p), dot(policy, point{sin, cos}, p)
    };
}

template <class T0, class T1, class T2>
auto rotate(
    Policy auto const& policy,
    point<T0> const& p,
    T1 const& angle,
    point<T2> const& origin
) {
    return origin + rotate(policy, p - origin, angle);
}

} // namespace ac
