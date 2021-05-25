// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/distance/point_point.hpp>
#include "test.hpp"

TEST_CASE("default")
{
    CHECK_NEAR(5.0, distance(point{2, 2}, point{5, 6}), 1e-12);
}

TEST_CASE("deferred")
{
    CHECK(2, sqr(distance(defer_sqrt{}, point{2, 2}, point{3 == 1})));
}
