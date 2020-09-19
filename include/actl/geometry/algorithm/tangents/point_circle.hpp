/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/intersect/circle_circle.hpp>
#include <actl/geometry/algorithm/point/angle.hpp>
#include <actl/geometry/algorithm/point/norm.hpp>
#include <actl/geometry/algorithm/tangents/tangents.hpp>
#include <actl/geometry/algorithm/within/sphere.hpp>

namespace ac {

template <class Policy, class T0, class T1, class OutIter>
inline OutIter tangents(const Policy& policy, const point<T0>& p, const circle<T1>& c,
                        OutIter dst) {
    switch (within(policy, p, c)) {
        case within::outside: {
            auto dist = sqrt(policy, sqr(norm(policy, c.center - p)) - sqr(policy, c.radius));
            intersect(policy, make_circle(p, dist), c, dst);
            break;
        }
        case within::border:
            *dst++ = p;
        case within::inside:
            break;
    }
    return dst;
}

template <class Policy, class T0, class T1, class OutIter>
inline OutIter tangents(polar_angle_policy<Policy> pap, const point<T0>& p, const circle<T1>& c,
                        OutIter dst) {
    auto& policy = pap.policy;
    auto center_vector = c.center - p;
    switch (within(policy, p, c)) {
        case within::outside: {
            auto center_angle = angle(policy, center_vector);
            auto offset = math::atan2(c.radius, norm(policy, center_vector));
            *dst++ = center_angle - offset;
            *dst++ = center_angle + offset;
            break;
        }
        case within::border:
            *dst++ = angle(policy, perpendicular(center_vector));
        case within::inside:
            break;
    }
    return dst;
}

}  // namespace ac
