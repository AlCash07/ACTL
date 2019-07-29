/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/2d/point2d.hpp>
#include <actl/geometry/algorithm/area/area.hpp>
#include <actl/geometry/polygon.hpp>

namespace ac {

/**
 * Doubled oriented area of a polygon : O(N).
 * http://geomalgorithms.com/a01-_area.html area2D_Polygon().
 */
template <class Policy, class T,
          enable_int_if<is_multi_point_v<T> && geometry_traits<T>::dimension == 2> = 0>
inline auto area(const Policy& policy, const T& poly) {
    auto it = cyclic_iterator{poly.begin(), poly};
    decltype(product(policy, it->x(), it->y())) res{};
    for (index i = 0; i < poly.size(); ++i) {
        res += product(policy, it->x(), it[1].y() - it[-1].y());
        ++it;
    }
    return res;
}

}  // namespace ac
