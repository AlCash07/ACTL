/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/distance/distance.hpp>
#include <actl/geometry/algorithm/point/norm.hpp>

namespace ac {

template <class NormPolicy>
struct distance_point_point : NormPolicy {};

template <class P = use_default>
using comparable_distance_point_point = distance_point_point<comparable_norm<P>>;

template <class P = use_default, class F = use_default>
using standard_distance_point_point = distance_point_point<standard_norm<P, F>>;

template <class Policy, index N, class T0, class T1>
inline auto distance(Policy&& policy, const point<T0, N>& p0, const point<T1, N>& p1) {
    return norm(policy, p0 - p1);
}

}  // namespace ac
