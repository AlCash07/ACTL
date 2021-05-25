// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/project/point_line.hpp>
#include "test.hpp"

TEST_CASE("scalar")
{
    ASSERT_ALMOST_EQUAL(
        0.5,
        project(
            line_scalar_policy{geometry_policy},
            point{2, 4},
            line<int>{{1, 1}, {5, 3}}),
        1e-12);
}

TEST_CASE("point")
{
    CHECK_NEAR(
        point{3, 2}, project(point{2, 4}, line<int>{{1, 1}, {5, 3}}), 1e-12);
}
