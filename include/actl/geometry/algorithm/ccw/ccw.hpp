/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>

namespace ac {

template <class AreaPolicy>
struct ccw_policy : AreaPolicy {};

template <class P = use_default>
using comparable_ccw = ccw_policy<comparable_area_points<P>>;

template <class Policy, class T, class U, enable_int_if_policy<Policy> = 0,
          geometry::enable_int_if_swap<T, U> = 0>
inline int ccw(const Policy& policy, const T& lhs, const U& rhs) {
    return -ccw(policy, rhs, lhs);
}

template <class T, class... Ts, disable_int_if_policy<T> = 0,
          enable_int_if<geometry_traits<T>::dimension == 2> = 0>
inline int ccw(const T& x, const Ts&... xs) {
    return ccw(geometry_policy, x, xs...);
}

}  // namespace ac
