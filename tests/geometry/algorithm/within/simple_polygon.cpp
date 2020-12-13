// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/within/simple_polygon.hpp>
#include <actl/test.hpp>
#include "geometry/polygons.hpp"

TEST("star") {
    auto poly = get_star_polygon();
    for (auto p : poly) {
        ASSERT_EQUAL(within::border, within(p, poly));
    }
    for (auto p : std::vector<point<int>>{{4, 2}, {-3, 2}}) {
        ASSERT_EQUAL(within::border, within(p, poly));
    }
    for (auto p : std::vector<point<int>>{{0, 0}, {0, 4}, {-3, 1}, {-2, -3}, {2, -3}, {3, 1}}) {
        ASSERT_EQUAL(within::inside, within(p, poly));
    }
    for (auto p : std::vector<point<int>>{{4, 1}, {1, 3}, {-4, 1}, {-3, -3}, {1, -3}, {0, -9}}) {
        ASSERT_EQUAL(within::outside, within(p, poly));
    }
}
