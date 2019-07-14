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

template <class P = use_default, class F = use_default, class NormPolicy = standard_norm<P, F>>
struct distance_point_plane : NormPolicy {};

template <class P, class F, class NP, index N, class T0, class T1>
inline auto distance(const distance_point_plane<P, F, NP>& policy, const point<T0, N>& p,
                     const plane<T1, N>& pl) {
    return static_cast<geometry::float_t<F, T0, T1>>(pl.template operator()<P>(p)) /
           norm(policy, pl.normal);
}

template <index N, class T0, class T1>
inline auto distance(use_default, const point<T0, N>& p, const plane<T1, N>& pl) {
    return distance(distance_point_plane{}, p, pl);
}

}  // namespace ac
