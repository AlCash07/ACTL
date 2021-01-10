// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/tangents/point_circle.hpp>
#include "test.hpp"

TEST_CASE("default") {
    circle<int> c{{0, 0}, 2};
    using vpi = std::vector<point<int>>;
    std::vector<point<double>> res;
    tangents(point{-1, 0}, c, std::back_inserter(res));
    CHECK(res.empty());
    tangents(point{-2, 0}, c, std::back_inserter(res));
    CHECK_NEAR(vpi{{-2, 0}}, res, 1e-12);
    res = {};
    tangents(point{2, -2}, c, std::back_inserter(res));
    CHECK_NEAR(vpi{{2, 0}, {0, -2}}, res, 1e-12);
}
