/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functions.hpp>
#include <actl/geometry/algorithm/area/point_point.hpp>

namespace ac {

template <class AreaPolicy>
struct collinear_policy : AreaPolicy {};

template <class P = use_default>
using comparable_collinear = collinear_policy<comparable_area_points<P>>;

template <class AP, class T0, class T1>
inline constexpr bool collinear(const collinear_policy<AP>& policy, const point2d<T0>& lhs,
                                const point2d<T1>& rhs) {
    return sgn(area(policy, lhs, rhs)) == 0;
}

template <class AP, int N, class T0, class T1>
inline bool collinear(const collinear_policy<AP>& policy, const point<N, T0>& lhs,
                      const point<N, T1>& rhs) {
    int i = 0;
    for (; i < N && lhs[i] == T0{0}; ++i) {
        if (rhs[i] != T1{0}) return false;
    }
    for (int j = i + 1; j < N; ++j) {
        if (!collinear(policy, make_point(lhs[i], lhs[j]), make_point(rhs[i], rhs[j])))
            return false;
    }
    return true;
}

template <int N, class T0, class T1>
inline constexpr bool collinear(const point<N, T0>& lhs, const point<N, T1>& rhs) {
    return collinear(comparable_collinear<>(), lhs, rhs);
}

}  // namespace ac
