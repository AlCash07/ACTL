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

/// Extreme (minimum orientation) point index with respect to the given
/// direction : O(log N). If there are two extreme vertices, the first one in
/// counter-clockwise order is taken. Joseph O'Rourke, Computational Geometry in
/// C (2nd Edition), p. 270.
template<class T, class Function>
auto extreme_vertex(
    Policy auto const& policy, convex_polygon<T> const& poly, Function direction
) {
    auto vertex_cmp = [&](auto i, auto j) {
        return orientation(policy, direction(*j), *j - *i);
    };
    auto is_extreme = [&](auto iter) {
        auto i = cyclic_iterator{poly, iter};
        auto v = vertex_cmp(i + 1, i);
        return std::pair{
            v != orientation2d::left &&
                vertex_cmp(i, i - 1) == orientation2d::left,
            v
        };
    };
    auto first = poly.begin();
    auto [first_ok, first_sgn] = is_extreme(first);
    if (first_ok)
        return first;
    for (auto last = poly.end(); first + 1 != last;) {
        auto middle = first + (last - first) / 2;
        auto [middle_ok, middle_sgn] = is_extreme(middle);
        if (middle_ok)
            return middle;
        if (first_sgn != middle_sgn ? first_sgn < middle_sgn
                                    : first_sgn == vertex_cmp(first, middle)) {
            last = middle;
        } else {
            first = middle;
            first_sgn = middle_sgn;
        }
    }
    return first;
}

template<class T, class Function>
auto extreme_vertex(convex_polygon<T> const& poly, Function direction) {
    return extreme_vertex(geometry_policy, poly, direction);
}

} // namespace ac
