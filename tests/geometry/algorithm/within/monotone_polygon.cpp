/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/within/monotone_polygon.hpp>
#include <actl/test.hpp>
#include "geometry/polygons.hpp"

TEST("trident") {
    auto poly = get_trident_polygon();
    for (auto& p : poly) p *= 2;
    for (auto p : poly) {
        ASSERT_EQUAL(1, within(p, poly));
    }
    for (auto p : std::vector<point<int>>{
             {-3, 0}, {-1, -1}, {1, -1}, {3, 0}, {4, 3}, {3, 5}, {1, 5}, {-1, 5}, {-3, 5}}) {
        ASSERT_EQUAL(1, within(p, poly));
    }
    for (auto p : std::vector<point<int>>{
             {-3, 1}, {-2, 1}, {-1, 0}, {0, -1}, {0, 7}, {1, 4}, {2, 1}, {3, 4}}) {
        ASSERT_EQUAL(2, within(p, poly));
    }
    for (auto p : std::vector<point<int>>{{-5, 3}, {-3, -1}, {-3, 6}, {-1, -2}, {-1, 6}, {0, -3}}) {
        ASSERT_EQUAL(0, within(p, poly));
    }
    for (auto p : std::vector<point<int>>{{0, 9}, {2, -1}, {2, 3}, {4, -1}, {4, 9}, {5, -1}}) {
        ASSERT_EQUAL(0, within(p, poly));
    }
}
