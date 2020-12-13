// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/tangents/point_circle.hpp>
#include <actl/test.hpp>

TEST("default") {
    circle<int> c{{0, 0}, 2};
    using vpi = std::vector<point<int>>;
    std::vector<point<double>> res;
    tangents(point{-1, 0}, c, std::back_inserter(res));
    ASSERT_TRUE(res.empty());
    tangents(point{-2, 0}, c, std::back_inserter(res));
    ASSERT_EQUAL(vpi{{-2, 0}}, res, 1e-12);
    res = {};
    tangents(point{2, -2}, c, std::back_inserter(res));
    ASSERT_EQUAL(vpi{{2, 0}, {0, -2}}, res, 1e-12);
}
