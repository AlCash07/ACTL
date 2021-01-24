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

template <class Policy, class T0, class T1, class OutIter>
OutIter intersect(const Policy& policy, const circle<T0>& lhs, const circle<T1>& rhs, OutIter dst) {
    auto centers_vector = rhs.center - lhs.center;
    auto centers_dist2 = dot(policy, centers_vector);
    int sgn0 = cmp3way(policy, centers_dist2, sqr(policy, lhs.radius - rhs.radius));
    int sgn1 = cmp3way(policy, sqr(policy, lhs.radius + rhs.radius), centers_dist2);
    if (sgn0 < 0 || sgn1 < 0)
        return dst;
    auto lradius2 = sqr(policy, lhs.radius);
    auto rradius2 = sqr(policy, rhs.radius);
    auto a = ratio(policy, lradius2 - rradius2, centers_dist2);
    auto projection = lhs.center + product(policy, ratio(policy, a + 1, 2), centers_vector);
    if (sgn0 == 0 || sgn1 == 0) {
        *dst++ = projection;
    } else {
        auto b = ratio(policy, lradius2 + rradius2, centers_dist2);
        auto offset = ratio(policy, sqrt(policy, product(policy, 2, b) - sqr(policy, a) - 1), 2);
        auto offset_vector = product(policy, offset, perpendicular(centers_vector));
        *dst++ = projection - offset_vector;
        *dst++ = projection + offset_vector;
    }
    return dst;
}

template <class Policy, class T0, class T1, class OutIter>
OutIter intersect(
    polar_angle_policy<Policy> pap, const circle<T0>& lhs, const circle<T1>& rhs, OutIter dst)  //
{
    auto centers_vector = rhs.center - lhs.center;
    auto centers_angle = angle(pap.policy, centers_vector);
    // TODO: implement using cosine theorem.
    return dst;
}

}  // namespace ac
