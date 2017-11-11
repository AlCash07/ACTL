/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/distance/point_point.hpp>
#include <actl/geometry/sphere.hpp>

namespace ac {

template <class P = use_default>
struct within_sphere : comparable_distance_point_point<P> {};

template <class P, int N, class T0, class T1>
inline int within(const within_sphere<P>& policy, const point<N, T0>& point,
                  const sphere<N, T1>& sphere) {
    return sgn(distance(policy, point, sphere.center), sphere.radius);
}

template <int N, class T0, class T1>
inline int within(use_default, const point<N, T0>& point, const sphere<N, T1>& sphere) {
    return within(within_sphere<>(), point, sphere);
}

}  // namespace ac
