// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/crosses/crosses.hpp>
#include <actl/geometry/algorithm/point/norm.hpp>
#include <actl/geometry/plane.hpp>
#include <actl/geometry/sphere.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1>
bool crosses(
    const Policy& policy, const plane<T0, N>& pl, const sphere<T1, N>& s)
{
    auto dist = abs(pl(policy, s.center));
    return !less(
        policy, dist, product(policy, s.radius, norm(policy, pl.normal)));
}

} // namespace ac
