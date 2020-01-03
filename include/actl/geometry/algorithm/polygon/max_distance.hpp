/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/scalar.hpp>
#include <actl/geometry/algorithm/distance/point_point.hpp>
#include <actl/geometry/algorithm/polygon/antipodal_vertices.hpp>
#include <actl/iterator/function_output_iterator.hpp>

namespace ac {

/**
 * Maximum distance between two convex polygon vertices : O(N).
 */
template <class Policy, class T>
inline auto max_distance(const Policy& policy, const convex_polygon<T>& poly) {
    decltype(distance(policy, poly[0], poly[0])) res = 0;
    antipodal_vertices(policy, poly, function_output_iterator{[&](auto pair) {
                           smax(res, distance(policy, *pair.first, *pair.second));
                       }});
    return res;
}

template <class T>
inline auto max_distance(const convex_polygon<T>& poly) {
    return max_distance(geometry_policy, poly);
}

}  // namespace ac
