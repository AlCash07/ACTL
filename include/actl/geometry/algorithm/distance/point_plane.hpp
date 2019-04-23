/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/point/norm.hpp>
#include <actl/geometry/plane.hpp>

namespace ac {

template <class P = use_default, class R = use_default, class NormPolicy = standard_norm<P>>
struct distance_point_plane : NormPolicy {};

template <class P, class R, class NP, int N, class T0, class T1>
inline auto distance(distance_point_plane<P, R, NP> policy, const point<T0, N>& point,
                     const plane<T1, N>& plane) {
    return static_cast<geometry::ratio_t<R, T0, T1>>(plane.template operator()<P>(point)) /
           norm(policy, plane.normal);
}

template <int N, class T0, class T1>
inline auto distance(use_default, const point<T0, N>& point, const plane<T1, N>& plane) {
    return distance(distance_point_plane<>{}, point, plane);
}

}  // namespace ac
