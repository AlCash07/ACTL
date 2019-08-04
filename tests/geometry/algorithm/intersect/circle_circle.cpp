/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/intersect/circle_circle.hpp>
#include <actl/test.hpp>

TEST("default") {
    circle<int> c0{{0, 0}, 17};
    using vpi = std::vector<point<int>>;
    std::vector<point<double>> res;
    auto dst = std::back_inserter(res);
    intersect(c0, circle<int>{{6, 0}, 10}, dst);
    ASSERT_TRUE(res.empty());
    intersect(c0, circle<int>{{28, 0}, 10}, dst);
    ASSERT_TRUE(res.empty());
    intersect(c0, circle<int>{{27, 0}, 10}, dst);
    ASSERT_EQUAL(vpi{{17, 0}}, res, 1e-12);
    res = {};
    intersect(c0, circle<int>{{21, 0}, 10}, dst);
    sort(res);
    ASSERT_EQUAL(vpi{{15, -8}, {15, 8}}, res, 1e-12);
}
