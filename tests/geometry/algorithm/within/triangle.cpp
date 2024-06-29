// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/within/triangle.hpp>
#include "test.hpp"

TEST_CASE("all cases") {
    polygon<point<int>> triangle{{0, 0}, {4, 2}, {2, 4}};
    for (auto p : triangle) {
        CHECK(within::border == within(p, triangle));
    }
    for (auto p : std::vector<point<int>>{{2, 1}, {3, 3}, {1, 2}}) {
        CHECK(within::border == within(p, triangle));
    }
    for (auto p : std::vector<point<int>>{{1, 1}, {3, 2}, {2, 3}}) {
        CHECK(within::inside == within(p, triangle));
    }
    for (auto p : std::vector<point<int>>{
             {-4, -2}, {-1, -2}, {5, 1}, {6, 3}, {4, 8}, {0, 6}
         }) {
        CHECK(within::outside == within(p, triangle));
    }
    for (auto p : std::vector<point<int>>{
             {-1, -1}, {1, 0}, {6, 1}, {3, 4}, {2, 5}, {-9, 7}
         }) {
        CHECK(within::outside == within(p, triangle));
    }
}
