/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/point.hpp>
#include <actl/test.hpp>

TEST("arithmetic") {
    point pi{1, 2};
    point pf{3.5f, 4.5f};
    point pd{5.5, 6.5};
    ASSERT_EQUAL(point{-1, -2}, -pi);
    ASSERT_EQUAL(point{4.5f, 6.5f}, pi + pf);
    ASSERT_EQUAL(point{4.5, 4.5}, pd - pi);
    ASSERT_EQUAL(point{3, 6}, pi * 3);
    ASSERT_EQUAL(point{4, 8}, 4 * pi);
    ASSERT_EQUAL(point{2.75, 3.25}, pd / 2);
    ASSERT_EQUAL(18.5, dot(pi, pd), 1e-12);
}

TEST("inplace arithmetic") {
    point p = {1, 2};
    ASSERT_EQUAL(point{3, 5}, p += {2, 3});
    ASSERT_EQUAL(point{2, 6}, p -= {1, -1});
    ASSERT_EQUAL(point{1, 3}, p /= 2);
    ASSERT_EQUAL(point{3, 9}, p *= 3);
    ASSERT_EQUAL(point{2, 3}, p = point<int>{point{2.5, 3.5}});
}

TEST("comparison") {
    ASSERT_EQUAL(point{1, 2}, point{1, 2});
    ASSERT_TRUE(point{1, 2} > point{0, 3});
    ASSERT_TRUE(point{1, 2} <= point{1, 3});
}
