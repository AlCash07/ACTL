// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/intersect/line_convex_polygon.hpp>
#include "test.hpp"

TEST_CASE("square") {
    convex_polygon<point<int>> poly{{2, 0}, {2, 2}, {0, 2}, {0, 0}};
    std::vector<point<double>> res;
    using vpi = std::vector<point<int>>;
    auto output = std::back_inserter(res);
    // horizontal lines:
    intersect(line<int>{{1, -1}, {1, 0}, true}, poly, output);
    CHECK(res.empty());
    intersect(line<int>{{1, 0}, {1, 0}, true}, poly, output);
    sort(res);
    CHECK_NEAR(vpi{{0, 0}, {2, 0}}, res, 1e-12);
    res = {};
    intersect(line<int>{{1, 1}, {1, 0}, true}, poly, output);
    sort(res);
    CHECK_NEAR(vpi{{0, 1}, {2, 1}}, res, 1e-12);
    res = {};
    intersect(line<int>{{1, 2}, {1, 0}, true}, poly, output);
    sort(res);
    CHECK_NEAR(vpi{{0, 2}, {2, 2}}, res, 1e-12);
    res = {};
    intersect(line<int>{{1, -2}, {1, 0}, true}, poly, output);
    CHECK(res.empty());
    // diagonal lines:
    intersect(line<int>{{0, -3}, {1, 1}, true}, poly, output);
    CHECK(res.empty());
    intersect(line<int>{{0, -2}, {1, 1}, true}, poly, output);
    // CHECK_NEAR(vpi{{2, 0}}, res, 1e-12);
    res = {};
    intersect(line<int>{{0, -1}, {1, 1}, true}, poly, output);
    sort(res);
    CHECK_NEAR(vpi{{1, 0}, {2, 1}}, res, 1e-12);
    res = {};
    intersect(line<int>{{0, 0}, {1, 1}, true}, poly, output);
    sort(res);
    CHECK_NEAR(vpi{{0, 0}, {2, 2}}, res, 1e-12);
}
