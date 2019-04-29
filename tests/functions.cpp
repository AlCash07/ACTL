/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/functions.hpp>
#include <actl/test.hpp>
#include <limits>

TEST("abs") {
    ASSERT_EQUAL(0, abs(0));
    constexpr auto int_max = std::numeric_limits<int>::max();
    ASSERT_EQUAL(int_max, abs(int_max));
    ASSERT_EQUAL(int_max, abs(-int_max));
    constexpr auto uint_max = std::numeric_limits<unsigned int>::max();
    ASSERT_EQUAL(uint_max, abs(uint_max));
}

TEST("sgn") {
    ASSERT_EQUAL(1, sgn(std::numeric_limits<int>::max()));
    ASSERT_EQUAL(-1, sgn(std::numeric_limits<int>::min()));
    ASSERT_EQUAL(1, sgn(std::numeric_limits<unsigned int>::max()));
    ASSERT_EQUAL(0, sgn(0));
    ASSERT_EQUAL(1, sgn(0, -1));
    ASSERT_EQUAL(-1, sgn(0u, 1u));
}

TEST("smax") {
    int x = 4;
    ASSERT_EQUAL(5, smax(x, 5));
    ASSERT_EQUAL(5, x);
    ASSERT_EQUAL(5, smax(x, 3));
    ASSERT_EQUAL(5, x);
}

TEST("smin") {
    int x = 4;
    ASSERT_EQUAL(4, smin(x, 5));
    ASSERT_EQUAL(4, x);
    ASSERT_EQUAL(3, smin(x, 3));
    ASSERT_EQUAL(3, x);
}

TEST("sqr") {
    ASSERT_EQUAL(0, sqr(0));
    ASSERT_EQUAL(4, sqr(2));
    ASSERT_EQUAL(4, sqr(-2));
}
