// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/intersect/circle_circle.hpp>
#include "test.hpp"

TEST_CASE("default")
{
    circle<int> c0{{0, 0}, 17};
    using vpi = std::vector<point<int>>;
    std::vector<point<double>> res;
    auto dst = std::back_inserter(res);
    intersect(c0, circle<int>{{6, 0}, 10}, dst);
    CHECK(res.empty());
    intersect(c0, circle<int>{{28, 0}, 10}, dst);
    CHECK(res.empty());
    intersect(c0, circle<int>{{27, 0}, 10}, dst);
    CHECK_NEAR(vpi{{17, 0}}, res, 1e-12);
    res = {};
    intersect(c0, circle<int>{{21, 0}, 10}, dst);
    sort(res);
    CHECK_NEAR(vpi{{15, -8}, {15, 8}}, res, 1e-12);
}
