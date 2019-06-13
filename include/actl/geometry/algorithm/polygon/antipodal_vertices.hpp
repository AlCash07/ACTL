/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/ccw/point_point.hpp>
#include <actl/geometry/polygon.hpp>
#include <actl/std/utility.hpp>

namespace ac {

template <class CcwPolicy = comparable_ccw<>>
struct antipodal_vertices_policy : CcwPolicy {};

/**
 * Outputs all candidates for the pairs of antipodal vertices (pairs of iterators).
 * The actual antipodal pair for a vertex is the last pair with this vertex in the output.
 */
template <class CP, class T, class OutputIterator>
inline auto antipodal_vertices(const antipodal_vertices_policy<CP>& policy,
                               const convex_polygon<T>& polygon, OutputIterator dst) {
    for (auto i = polygon.begin(), j = i + 1; j != polygon.end(); ++i) {
        for (; j != polygon.end(); ++j) {
            *dst++ = std::pair{i, j};
            if (ccw(policy, i[1] - i[0], polygon.cyclic(j)[1] - j[0]) >= 0) break;
        }
    }
    return dst;
}

template <class T, class OutputIterator>
inline auto antipodal_vertices(const convex_polygon<T>& polygon, OutputIterator dst) {
    return antipodal_vertices(antipodal_vertices_policy<>{}, polygon, dst);
}

}  // namespace ac
