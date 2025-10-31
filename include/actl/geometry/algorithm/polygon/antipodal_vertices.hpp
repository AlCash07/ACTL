// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/orientation/point_point.hpp>
#include <actl/geometry/polygon.hpp>
#include <actl/std/utility.hpp>

namespace ac {

/// Outputs all candidates for the pairs of antipodal vertices (pairs of
/// iterators). The actual antipodal pair for a vertex is the last pair with
/// this vertex in the output.
template<typename T, typename OutIter>
auto antipodal_vertices(
    Policy auto const& policy, convex_polygon<T> const& poly, OutIter output
) {
    for (auto i = poly.begin(), j = i + 1; j != poly.end(); ++i) {
        for (; j != poly.end(); ++j) {
            *output++ = std::pair{i, j};
            auto vecj = *(j + 1 != poly.end() ? j + 1 : poly.begin()) - *j;
            if (!left_turn(policy, i[1] - *i, vecj))
                break;
        }
    }
    return output;
}

template<typename T, typename OutIter>
auto antipodal_vertices(convex_polygon<T> const& poly, OutIter output) {
    return antipodal_vertices(geometry_policy, poly, output);
}

} // namespace ac
