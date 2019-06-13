/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/polygon/extreme_vertex.hpp>
#include <actl/std/utility.hpp>

namespace ac {

template <class ExtremeVertexPolicy = extreme_vertex_policy<>>
struct tangents_point_convex_polygon : ExtremeVertexPolicy {};

/**
 * Returns pair of iterators to polygon vertices tangent w.r.t. the given point.
 * Point must be outside the polygon.
 */
template <class EVP, class T0, class T1>
inline auto tangents(const tangents_point_convex_polygon<EVP>& policy, const point<T0>& point,
                     const polygon<T1>& polygon) {
    return std::pair{
        extreme_vertex(policy, polygon, [&point](const auto& query) { return query - point; }),
        extreme_vertex(policy, polygon, [&point](const auto& query) { return point - query; })};
}

template <class T0, class T1>
inline auto tangents(use_default, const point<T0>& point, const polygon<T1>& polygon) {
    return tangents(tangents_point_convex_polygon<>{}, point, polygon);
}

}  // namespace ac
