/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/polygon.hpp>
#include <actl/geometry/traits/aliases.hpp>
#include <actl/geometry/traits/policy.hpp>

namespace ac {

template <class P = use_default>
struct area_polygon : geometry::policy {};

/**
 * Doubled oriented area of a polygon : O(N).
 * http://geomalgorithms.com/a01-_area.html area2D_Polygon().
 */
template <class P, class T, class X = geometry::product_t<P, polygon<T>>>
inline X area(area_polygon<P>, const polygon<T>& polygon) {
    X res{};
    auto it = polygon.cyclic_begin();
    for (index i = 0; i < polygon.size(); ++i) {
        res += it->x() * static_cast<X>(it[-1].y() - it[1].y());
        ++it;
    }
    return res;
}

template <class Range>
inline auto area(use_default, const polygon<Range>& polygon) {
    return area(area_polygon<>(), polygon);
}

}  // namespace ac
