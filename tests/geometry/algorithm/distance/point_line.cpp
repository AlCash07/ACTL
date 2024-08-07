// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/distance/point_line.hpp>
#include "test.hpp"

TEST_CASE("horizontal segment") {
    segment<int> l{{2, 0}, {7, 0}};
    CHECK_NEAR(0.0, distance(l, point{6, 0}), 1e-12);
    CHECK_NEAR(2.0, distance(point{3, 2}, l), 1e-12);
    CHECK_NEAR(2.0, distance(point{5, -2}, l), 1e-12);
    CHECK_NEAR(5.0, distance(point{-1, 4}, l), 1e-12);
    CHECK_NEAR(5.0, distance(point{11, -3}, l), 1e-12);
}

TEST_CASE("horizontal line") {
    line<int> l{{2, 0}, {7, 0}};
    CHECK_NEAR(0.0, distance(l, point{7, 0}), 1e-12);
    CHECK_NEAR(4.0, distance(point{-1, 4}, l), 1e-12);
    CHECK_NEAR(3.0, distance(point{11, -3}, l), 1e-12);
}
