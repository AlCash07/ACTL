/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/point/angle_compare.hpp>
#include <actl/geometry/multi_point.hpp>
#include <algorithm>

namespace ac {

template <class P = use_default>
struct sort_by_angle_policy : geometry::policy {};

/**
 * Sorts 2d points by polar angle from 0 to 2 * pi. Points at origin go first.
 */
template <class P, class T0, class T1>
void sort_by_angle(sort_by_angle_policy<P>, multi_point<T0>& points, const point<T1>& origin) {
    auto first = points.begin(), last = points.end();
    static_assert(geometry_traits<multi_point<T0>>::dimension == 2, "incompatible dimension");
    using reference = reference_t<multi_point<T0>>;
    first = std::partition(first, last, [&origin](reference point) { return point == origin; });
    auto pivot = std::partition(first, last,
                                [&origin](reference point) { return y_compare(origin, point); });
    auto angle_cmp = make_angle_compare<P>(origin);
    std::sort(first, pivot, angle_cmp);
    std::sort(pivot, last, angle_cmp);
}

template <class T0, class T1>
inline auto sort_by_angle(multi_point<T0>& dst, const point<T1>& origin) {
    return sort_by_angle(sort_by_angle_policy<>{}, dst, origin);
}

}  // namespace ac
