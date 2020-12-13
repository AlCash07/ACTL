// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/within/sphere.hpp>
#include <actl/test.hpp>

TEST("2d") {
    circle<int> c{{0, 0}, 5};
    for (auto p : std::vector<point<int>>{{0, 0}, {-4, -2}, {3, -3}}) {
        ASSERT_EQUAL(within::inside, within(p, c));
    }
    for (auto p : std::vector<point<int>>{{-5, 0}, {-4, 3}, {3, -4}}) {
        ASSERT_EQUAL(within::border, within(p, c));
    }
    for (auto p : std::vector<point<int>>{{-3, 5}, {1, 5}, {6, 0}}) {
        ASSERT_EQUAL(within::outside, within(p, c));
    }
}
