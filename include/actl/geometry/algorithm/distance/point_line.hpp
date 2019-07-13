/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/area/point_line.hpp>
#include <actl/geometry/algorithm/distance/point_point.hpp>
#include <actl/geometry/line.hpp>
#include <actl/numeric/functions.hpp>

namespace ac {

template <class P = use_default, class F = use_default, class NormPolicy = use_default,
          class AreaPolicy = standard_area_points<P>>
struct distance_point_line : deduce_t<NormPolicy, standard_norm<P>> {
    AreaPolicy area_policy;
};

template <class P, class F, class NP, class AP, index N, class T0, class T1, class K>
inline auto distance(distance_point_line<P, F, NP, AP> policy, const point<T0, N>& point,
                     const line<T1, N, K>& line) {
    if (line.start_kind() != endpoint::free && dot<P>(point - line.start, line.vector) <= 0)
        return norm(policy, point - line.start);
    if (line.end_kind() != endpoint::free && dot<P>(point - line.end(), line.vector) >= 0)
        return norm(policy, point - line.end());
    return static_cast<geometry::float_t<F, T0, T1>>(abs(area(policy.area_policy, point, line))) /
           norm(policy, line.vector);
}

template <index N, class T0, class T1, class K>
inline auto distance(use_default, const point<T0, N>& point, const line<T1, N, K>& line) {
    return distance(distance_point_line<>{}, point, line);
}

}  // namespace ac
