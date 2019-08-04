/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/within/triangle.hpp>
#include <actl/test.hpp>

TEST("all cases") {
    polygon<point<int>> triangle{{0, 0}, {4, 2}, {2, 4}};
    for (auto p : triangle) {
        ASSERT_EQUAL(within::border, within(p, triangle));
    }
    for (auto p : std::vector<point<int>>{{2, 1}, {3, 3}, {1, 2}}) {
        ASSERT_EQUAL(within::border, within(p, triangle));
    }
    for (auto p : std::vector<point<int>>{{1, 1}, {3, 2}, {2, 3}}) {
        ASSERT_EQUAL(within::inside, within(p, triangle));
    }
    for (auto p : std::vector<point<int>>{{-4, -2}, {-1, -2}, {5, 1}, {6, 3}, {4, 8}, {0, 6}}) {
        ASSERT_EQUAL(within::outside, within(p, triangle));
    }
    for (auto p : std::vector<point<int>>{{-1, -1}, {1, 0}, {6, 1}, {3, 4}, {2, 5}, {-9, 7}}) {
        ASSERT_EQUAL(within::outside, within(p, triangle));
    }
}
