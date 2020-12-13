// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/orientation/point_line.hpp>
#include <actl/test.hpp>

TEST("default") {
    line<int> l{{1, 1}, {3, 2}};
    ASSERT_EQUAL(orientation2d::collinear, orientation(point{-1, 0}, l));
    ASSERT_EQUAL(orientation2d::left, orientation(l, point{3, 3}));
    ASSERT_EQUAL(orientation2d::right, orientation(l, point{3, 1}));
}
