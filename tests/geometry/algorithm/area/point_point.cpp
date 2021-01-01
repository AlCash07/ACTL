// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/geometry/algorithm/area/point_point.hpp>

TEST_CASE("2d") {
    point p0{3, 0};
    point p1{2, 1};
    CHECK(3 == area(p0, p1));
    CHECK(-3 == area(p1, p0));
}

TEST_CASE("3d") {
    CHECK_NEAR(2.9580398915498103, area(point{-2, 1, 1}, point{1, -1, 2}) / 2, 1e-12);
}
