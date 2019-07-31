/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/nearest/multi_point.hpp>
#include <actl/test.hpp>

TEST("grid") {
    std::vector<point<int>> grid;
    for (int i : irange(222)) {
        for (int j : irange(222)) {
            grid.emplace_back(i * 3, j * 3);
        }
    }
    point p0{178 * 3, 42 * 3};
    point p1 = p0 + point{1, -1};
    grid.emplace_back(p1);
    auto res = nearest(grid);
    using std::swap;
    if (res.first > res.second) swap(res.first, res.second);
    ASSERT_EQUAL(p0, res.first);
    ASSERT_EQUAL(p1, res.second);
}
