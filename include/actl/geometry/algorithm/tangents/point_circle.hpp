// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/intersect/circle_circle.hpp>
#include <actl/geometry/algorithm/point/angle.hpp>
#include <actl/geometry/algorithm/point/norm.hpp>
#include <actl/geometry/algorithm/tangents/tangents.hpp>
#include <actl/geometry/algorithm/within/sphere.hpp>

namespace ac {

template<typename T0, typename T1, typename OutIter>
OutIter tangents(
    Policy auto const& policy,
    point<T0> const& p,
    circle<T1> const& c,
    OutIter output
) {
    switch (within(policy, p, c)) {
        case within::outside: {
            auto dist = sqrt(
                policy, sqr(norm(policy, c.center - p)) - sqr(policy, c.radius)
            );
            intersect(policy, make_circle(p, dist), c, output);
            break;
        }
        case within::border:
            *output++ = p;
        case within::inside:
            break;
    }
    return output;
}

template<typename P, typename T0, typename T1, typename OutIter>
OutIter tangents(
    polar_angle_policy<P> pap,
    point<T0> const& p,
    circle<T1> const& c,
    OutIter output
) {
    auto& policy = pap.policy;
    auto center_vector = c.center - p;
    switch (within(policy, p, c)) {
        case within::outside: {
            auto center_angle = angle(policy, center_vector);
            auto offset = atan2(c.radius, norm(policy, center_vector));
            *output++ = center_angle - offset;
            *output++ = center_angle + offset;
            break;
        }
        case within::border:
            *output++ = angle(policy, perpendicular(center_vector));
        case within::inside:
            break;
    }
    return output;
}

} // namespace ac
