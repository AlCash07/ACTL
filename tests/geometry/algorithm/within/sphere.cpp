// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/within/sphere.hpp>
#include "test.hpp"

TEST_CASE("2d") {
    circle<int> c{{0, 0}, 5};
    for (auto p : std::vector<point<int>>{{0, 0}, {-4, -2}, {3, -3}}) {
        CHECK(within::inside == within(p, c));
    }
    for (auto p : std::vector<point<int>>{{-5, 0}, {-4, 3}, {3, -4}}) {
        CHECK(within::border == within(p, c));
    }
    for (auto p : std::vector<point<int>>{{-3, 5}, {1, 5}, {6, 0}}) {
        CHECK(within::outside == within(p, c));
    }
}
