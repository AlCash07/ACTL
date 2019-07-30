/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/within/convex_monotone_polygon.hpp>
#include <actl/test.hpp>
#include "geometry/polygons.hpp"

TEST("octagon") {
    auto poly = get_octagon();
    for (auto p : poly) {
        ASSERT_EQUAL(1, within(p, poly));
    }
    for (auto p : std::vector<point<int>>{
             {-2, -2}, {2, -2}, {2, 2}, {-2, 2}, {0, -3}, {3, 0}, {0, 3}, {-3, 0}}) {
        ASSERT_EQUAL(1, within(p, poly));
    }
    for (auto p : std::vector<point<int>>{
             {0, 0}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}}) {
        ASSERT_EQUAL(2, within(p, poly));
    }
    for (auto p : std::vector<point<int>>{{-6, -2}, {6, 2}, {0, -4}, {4, 0}, {0, 4}, {-4, 0}}) {
        ASSERT_EQUAL(0, within(p, poly));
    }
    for (auto p : std::vector<point<int>>{
             {-3, -2}, {-2, -3}, {2, -3}, {3, -2}, {3, 2}, {2, 3}, {-2, 3}, {-3, 2}}) {
        ASSERT_EQUAL(0, within(p, poly));
    }
}
