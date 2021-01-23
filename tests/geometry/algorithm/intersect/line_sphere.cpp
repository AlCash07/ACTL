// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/intersect/line_sphere.hpp>
#include "test.hpp"

TEST_CASE("2d line all cases") {
    circle<int> c{{0, 0}, 5};
    using vpd = std::vector<point<double>>;
    vpd res;
    intersect(line<int>{{6, 0}, {6, 1}}, c, std::back_inserter(res));
    CHECK(res.empty());
    intersect(line<int>{{5, 0}, {5, 1}}, c, std::back_inserter(res));
    CHECK_NEAR(vpd{{5.0, 0.0}}, res, 1e-12);
    res = {};
    intersect(line<int>{{4, 0}, {4, 1}}, c, std::back_inserter(res));
    sort(res);
    CHECK_NEAR(vpd{{4.0, -3.0}, {4.0, 3.0}}, res, 1e-12);
}

TEST_CASE("2d ray") {
    circle<int> c{{0, 0}, 5};
    using vpd = std::vector<point<double>>;
    vpd res;
    intersect(ray<int>{{4, -2}, {4, 2}}, c, std::back_inserter(res));
    CHECK_NEAR(vpd{{4.0, 3.0}}, res, 1e-12);
    res = {};
    intersect(ray<int>{{4, 2}, {4, -2}}, c, std::back_inserter(res));
    CHECK_NEAR(vpd{{4.0, -3.0}}, res, 1e-12);
}
