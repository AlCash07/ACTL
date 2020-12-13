// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/within/plane.hpp>
#include <actl/test.hpp>

TEST("3d") {
    plane<int> pl{{1, 2, 3}, {1, 1, 1}};
    for (auto p : std::vector<point<int, 3>>{{1, 1, 1}, {4, 1, 0}, {-1, 2, 1}, {1, 4, -1}}) {
        ASSERT_EQUAL(within::border, within(p, pl));
    }
    for (auto p : std::vector<point<int, 3>>{{0, 1, 1}, {4, 0, 0}, {-1, 2, 2}, {1, 5, -1}}) {
        ASSERT_EQUAL(within::outside, within(p, pl));
    }
}
