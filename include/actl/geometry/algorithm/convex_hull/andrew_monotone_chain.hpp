/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/ccw/point_point.hpp>
#include <actl/geometry/polygon.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/traits/type_traits.hpp>

namespace ac {

template <bool Monotone = false, class Point = use_default, class CcwPolicy = comparable_ccw<>>
struct andrew_monotone_chain : CcwPolicy {};

namespace detail {

template <class T>
static void set_right(std::bool_constant<true>, T& dst, int right) { dst.right_ = right; }

template <class T>
static void set_right(std::bool_constant<false>, T&, int) {}

}  // namespace detail

/**
 * Convex hull of a set of 2d points : O(N log N).
 * https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
 * Implementation reference: https://github.com/stjepang/snippets/blob/master/convex_hull.cpp
 */
template <bool M, class P, class AP, class T,
          enable_int_if<geometry_traits<multi_point<T>>::dimension == 2> = 0>
inline auto convex_hull(andrew_monotone_chain<M, P, AP> policy, multi_point<T>& points) {
    sort(points);
    using point_t = deduce_type_t<P, typename geometry_traits<multi_point<T>>::point>;
    std::conditional_t<M, convex_monotone_polygon<point_t>, convex_polygon<point_t>> hull;
    hull.reserve(points.size() + 1);
    for (int phase = 0; phase < 2; ++phase) {  // Lower, then upper chain.
        auto start = hull.size();
        detail::set_right(std::bool_constant<M>(), hull, static_cast<int>(start));
        for (const auto& point : points) {
            while (hull.size() >= start + 2 &&
                   ccw(policy, point, hull.back(), hull[hull.size() - 2]) <= 0)
                hull.pop_back();
            hull.push_back(point);
        }
        hull.pop_back();
        reverse(points);
    }
    if (hull.size() == 2 && hull[0] == hull[1]) hull.pop_back();
    return hull;
}

template <class T, enable_int_if<geometry_traits<multi_point<T>>::dimension == 2> = 0>
inline auto convex_hull(use_default, multi_point<T>& points) {
    return convex_hull(andrew_monotone_chain<>{}, points);
}

}  // namespace ac
