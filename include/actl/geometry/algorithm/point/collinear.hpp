/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/compare.hpp>
#include <actl/geometry/algorithm/area/point_point.hpp>

namespace ac {

template <class AreaPolicy>
struct collinear_policy : AreaPolicy {};

template <class P = use_default>
using comparable_collinear = collinear_policy<comparable_area_points<P>>;

template <class Policy, class T0, class T1>
inline constexpr bool collinear(Policy&& policy, const point<T0>& lhs, const point<T1>& rhs) {
    return equal(policy, area(policy, lhs, rhs), 0);
}

template <class Policy, index N, class T0, class T1>
inline bool collinear(Policy&& policy, const point<T0, N>& lhs, const point<T1, N>& rhs) {
    index i = 0;
    for (; i < N && equal(policy, lhs[i], 0); ++i) {
        if (!equal(policy, rhs[i], 0)) return false;
    }
    for (index j = i + 1; j < N; ++j) {
        if (!collinear(policy, point{lhs[i], lhs[j]}, point{rhs[i], rhs[j]}))
            return false;
    }
    return true;
}

template <index N, class T0, class T1>
inline constexpr bool collinear(const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return collinear(geometry_policy, lhs, rhs);
}

}  // namespace ac
