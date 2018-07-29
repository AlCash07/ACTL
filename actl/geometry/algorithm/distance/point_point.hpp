/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/point/norm.hpp>

namespace ac {

template <class NormPolicy>
struct distance_point_point : NormPolicy {};

template <class P = use_default>
using comparable_distance_point_point = distance_point_point<comparable_norm<P>>;

template <class P = use_default, class S = use_default>
using standard_distance_point_point = distance_point_point<standard_norm<P, S>>;

template <class NP, int N, class T0, class T1>
inline auto distance(const distance_point_point<NP>& policy, const point<T0, N>& lhs,
                     const point<T1, N>& rhs) {
    return norm(policy, lhs - rhs);
}

template <int N, class T0, class T1>
inline auto distance(use_default, const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return distance(standard_distance_point_point<>(), lhs, rhs);
}

}  // namespace ac
