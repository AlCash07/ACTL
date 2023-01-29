// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/distance/point_point.hpp>
#include <actl/geometry/algorithm/polygon/antipodal_vertices.hpp>
#include <actl/iterator/function_output_iterator.hpp>
#include <actl/operation/scalar/all.hpp>

namespace ac {

/// Maximum distance between two convex polygon vertices : O(N).
template <class T>
auto max_distance(Policy auto const& policy, convex_polygon<T> const& poly)
{
    decltype(distance(policy, poly[0], poly[0])) res = 0;
    antipodal_vertices(
        policy,
        poly,
        function_output_iterator{
            [&](auto pair)
            {
                max(inout{res}, distance(policy, *pair.first, *pair.second));
            }});
    return res;
}

template <class T>
auto max_distance(convex_polygon<T> const& poly)
{
    return max_distance(geometry_policy, poly);
}

} // namespace ac
