// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/orientation/convex_polygon.hpp>
#include "test.hpp"

TEST_CASE("default")
{
    convex_polygon<point<int>> poly{{1, 0}, {1, 1}, {0, 1}};
    CHECK(orientation2d::left == orientation(poly));
}
