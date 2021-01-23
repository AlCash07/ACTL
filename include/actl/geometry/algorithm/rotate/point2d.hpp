// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/rotate/rotate.hpp>
#include <actl/numeric/math.hpp>

namespace ac {

template <class Policy, class T0, class T1>
auto rotate(const Policy& policy, const point<T0>& p, const T1& angle) {
    auto sin = sin(angle);
    auto cos = cos(angle);
    return point{dot(policy, point{cos, sin}, p), dot(policy, point{sin, cos}, p)};
}

template <class Policy, class T0, class T1, class T2>
auto rotate(const Policy& policy, const point<T0>& p, const T1& angle, const point<T2>& origin) {
    return origin + rotate(policy, p - origin, angle);
}

}  // namespace ac
