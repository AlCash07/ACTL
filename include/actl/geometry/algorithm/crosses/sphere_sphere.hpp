// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/crosses/crosses.hpp>
#include <actl/geometry/algorithm/distance/point_point.hpp>
#include <actl/geometry/sphere.hpp>

namespace ac {

template<index N, typename TL, typename TR>
bool crosses(
    Policy auto const& policy, sphere<TL, N> const& l, sphere<TR, N> const& r
) {
    auto centers_dist = distance(policy, l.center, r.center);
    return !less(policy, centers_dist, abs(l.radius - r.radius)) &&
           !less(policy, l.radius + r.radius, centers_dist);
}

} // namespace ac
