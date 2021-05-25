// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/nearest/multi_point.hpp>
#include "test.hpp"

TEST_CASE("grid")
{
    std::vector<point<int>> grid;
    for (int i : irange(222))
    {
        for (int j : irange(222))
        {
            grid.emplace_back(i * 3, j * 3);
        }
    }
    point p0{178 * 3, 42 * 3};
    point p1 = p0 + point{1, -1};
    grid.emplace_back(p1);
    auto res = nearest(defer_sqrt{}, grid);
    using std::swap;
    if (res.first > res.second)
        swap(res.first, res.second);
    CHECK(p0 == res.first);
    CHECK(p1 == res.second);
}
