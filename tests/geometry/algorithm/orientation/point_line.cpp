// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/orientation/point_line.hpp>
#include "test.hpp"

TEST_CASE("default") {
    line<int> l{{1, 1}, {3, 2}};
    CHECK(orientation2d::collinear == orientation(point{-1, 0}, l));
    CHECK(orientation2d::left == orientation(l, point{3, 3}));
    CHECK(orientation2d::right == orientation(l, point{3, 1}));
}
