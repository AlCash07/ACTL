/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/point/norm.hpp>
#include <actl/geometry/plane.hpp>
#include <actl/geometry/sphere.hpp>

namespace ac {

template <class P = use_default, class NormPolicy = comparable_norm<P>>
struct crosses_plane_sphere : NormPolicy {};

template <class P, class NP, index N, class T0, class T1>
inline bool crosses(const crosses_plane_sphere<P, NP>& policy, const plane<T0, N>& pl,
                    const sphere<T1, N>& s) {
    auto dist = adl::abs(pl(policy, s.center));
    return s.radius * norm(policy, pl.normal) <= dist;
}

template <index N, class T0, class T1>
inline bool crosses(use_default, const plane<T0, N>& pl, const sphere<T1, N>& s) {
    return crosses(crosses_plane_sphere{}, pl, s);
}

}  // namespace ac
