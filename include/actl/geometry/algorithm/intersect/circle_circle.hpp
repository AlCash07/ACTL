// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/intersect/intersect.hpp>
#include <actl/geometry/algorithm/point/angle.hpp>
#include <actl/geometry/sphere.hpp>
#include <actl/numeric/math.hpp>

namespace ac {

template<typename TL, typename TR, typename OutIter>
OutIter intersect(
    Policy auto const& policy,
    circle<TL> const& lhs,
    circle<TR> const& rhs,
    OutIter output
) {
    auto centers_vector = rhs.center - lhs.center;
    auto centers_dist2 = dot(policy, centers_vector);
    int sgn0 = compare3way(
        policy, centers_dist2, squared(policy, lhs.radius - rhs.radius)
    );
    int sgn1 = compare3way(
        policy, squared(policy, lhs.radius + rhs.radius), centers_dist2
    );
    if (sgn0 < 0 || sgn1 < 0)
        return output;
    auto lradius2 = squared(policy, lhs.radius);
    auto rradius2 = squared(policy, rhs.radius);
    auto a = ratio(policy, lradius2 - rradius2, centers_dist2);
    auto projection =
        lhs.center + product(policy, ratio(policy, a + 1, 2), centers_vector);
    if (sgn0 == 0 || sgn1 == 0) {
        *output++ = projection;
    } else {
        auto b = ratio(policy, lradius2 + rradius2, centers_dist2);
        auto offset = ratio(
            policy,
            square_root(policy, product(policy, 2, b) - squared(policy, a) - 1),
            2
        );
        auto offset_vector =
            product(policy, offset, perpendicular(centers_vector));
        *output++ = projection - offset_vector;
        *output++ = projection + offset_vector;
    }
    return output;
}

template<typename TL, typename TR, typename OutIter>
OutIter intersect(
    polar_angle_policy<P> pap,
    circle<TL> const& l,
    circle<TR> const& r,
    OutIter output
) {
    auto centers_vector = r.center - l.center;
    auto centers_angle = angle(pap.policy, centers_vector);
    // TODO: implement using cosine theorem.
    return output;
}

} // namespace ac
