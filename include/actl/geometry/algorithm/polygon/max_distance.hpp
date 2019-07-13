/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/distance/point_point.hpp>
#include <actl/geometry/algorithm/polygon/antipodal_vertices.hpp>
#include <actl/iterator/function_output_iterator.hpp>
#include <actl/numeric/functions.hpp>

namespace ac {

template <class DistancePolicy = use_default, class AntipodalPolicy = antipodal_vertices_policy<>>
struct max_distance_policy : AntipodalPolicy {
    deduce_t<DistancePolicy, comparable_distance_point_point<>> distance_policy;
};

/**
 * Maximum distance between two convex polygon vertices : O(N).
 */
template <class DP, class AP, class T>
inline auto max_distance(const max_distance_policy<DP, AP>& policy,
                         const convex_polygon<T>& polygon) {
    decltype(distance(policy.distance_policy, *polygon.begin(), *polygon.begin())) res{};
    auto update_res = [&](auto pair) {
        smax(res, distance(policy.distance_policy, *pair.first, *pair.second));
    };
    antipodal_vertices(policy, polygon, function_output_iterator(update_res));
    return res;
}

template <class T>
inline auto max_distance(const convex_polygon<T>& polygon) {
    return max_distance(max_distance_policy{}, polygon);
}

}  // namespace ac
