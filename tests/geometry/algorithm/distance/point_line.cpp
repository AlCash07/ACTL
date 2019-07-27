/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/distance/point_line.hpp>
#include <actl/test.hpp>

TEST("horizontal segment") {
    segment<int> l{{2, 0}, {7, 0}};
    ASSERT_EQUAL(0.0, distance(l, point{6, 0}), 1e-12);
    ASSERT_EQUAL(2.0, distance(point{3, 2}, l), 1e-12);
    ASSERT_EQUAL(2.0, distance(point{5, -2}, l), 1e-12);
    ASSERT_EQUAL(5.0, distance(point{-1, 4}, l), 1e-12);
    ASSERT_EQUAL(5.0, distance(point{11, -3}, l), 1e-12);
}

TEST("horizontal line") {
    line<int> l{{2, 0}, {7, 0}};
    ASSERT_EQUAL(0.0, distance(l, point{7, 0}), 1e-12);
    ASSERT_EQUAL(4.0, distance(point{-1, 4}, l), 1e-12);
    ASSERT_EQUAL(3.0, distance(point{11, -3}, l), 1e-12);
}
