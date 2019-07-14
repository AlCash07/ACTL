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

namespace ac {

template <class P = use_default, class F = use_default, class NormPolicy = use_default,
          class AreaPolicy = standard_area_points<P, F>>
struct distance_point_line : deduce_t<NormPolicy, standard_norm<P, F>> {
    AreaPolicy area_policy;
};

template <class P, class F, class NP, class AP, index N, class T0, class T1, class K>
inline auto distance(const distance_point_line<P, F, NP, AP>& policy, const point<T0, N>& p,
                     const line<T1, N, K>& l) {
    if (l.start_kind() != endpoint::free && dot<P>(p - l.start, l.vector) <= 0)
        return norm(policy, p - l.start);
    if (l.end_kind() != endpoint::free && dot<P>(p - l.end(), l.vector) >= 0)
        return norm(policy, p - l.end());
    return static_cast<geometry::float_t<F, T0, T1>>(adl::abs(area(policy.area_policy, p, l))) /
           norm(policy, l.vector);
}

template <index N, class T0, class T1, class K>
inline auto distance(use_default, const point<T0, N>& p, const line<T1, N, K>& l) {
    return distance(distance_point_line{}, p, l);
}

}  // namespace ac
