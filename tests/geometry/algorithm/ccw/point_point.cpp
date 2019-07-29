/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/ccw/point_point.hpp>
#include <actl/test.hpp>

TEST("default") {
    point a{6, 2}, b{8, 7}, c{1, 9};
    ASSERT_EQUAL(-1, ccw(a, b, c));
    ASSERT_EQUAL(-1, ccw(b, c, a));
    ASSERT_EQUAL(-1, ccw(c, a, b));
    ASSERT_EQUAL(1, ccw(a, c, b));
    ASSERT_EQUAL(1, ccw(b, a, c));
    ASSERT_EQUAL(1, ccw(c, b, a));
}

TEST("zero") {
    point a{2, 1};
    ASSERT_EQUAL(0, ccw(2 * a, 5 * a));
    ASSERT_EQUAL(0, ccw(-a, 3 * a));
}
