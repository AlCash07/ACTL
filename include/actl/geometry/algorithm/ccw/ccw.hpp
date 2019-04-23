/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>
#include <actl/geometry/traits/policy.hpp>

namespace ac {

template <class AreaPolicy>
struct ccw_policy : AreaPolicy {};

template <class P = use_default>
using comparable_ccw = ccw_policy<comparable_area_points<P>>;

template <class T, class... Ts, class = geometry::disable_if_policy_t<T>,
          class = std::enable_if_t<geometry_traits<T>::dimension == 2>>
inline auto ccw(const T& arg, const Ts&... args) {
    return ccw(comparable_ccw<>{}, arg, args...);
}

}  // namespace ac
