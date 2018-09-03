/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/functions.hpp>
#include <actl/test.hpp>
#include <limits>

using namespace ac;

TEST("abs") {
    ASSERT_EQUAL(abs(0), 0);
    constexpr auto int_max = std::numeric_limits<int>::max();
    ASSERT_EQUAL(abs(int_max), int_max);
    ASSERT_EQUAL(abs(-int_max), int_max);
    constexpr auto uint_max = std::numeric_limits<unsigned int>::max();
    ASSERT_EQUAL(abs(uint_max), uint_max);
}

TEST("sgn") {
    ASSERT_EQUAL(sgn(std::numeric_limits<int>::max()), 1);
    ASSERT_EQUAL(sgn(std::numeric_limits<int>::min()), -1);
    ASSERT_EQUAL(sgn(std::numeric_limits<unsigned int>::max()), 1);
    ASSERT_EQUAL(sgn(0), 0);
    ASSERT_EQUAL(sgn(0, -1), 1);
    ASSERT_EQUAL(sgn(0u, 1u), -1);
}

TEST("smax") {
    int x = 4;
    ASSERT_EQUAL(smax(x, 5), 5);
    ASSERT_EQUAL(x, 5);
    ASSERT_EQUAL(smax(x, 3), 5);
    ASSERT_EQUAL(x, 5);
}

TEST("smin") {
    int x = 4;
    ASSERT_EQUAL(smin(x, 5), 4);
    ASSERT_EQUAL(x, 4);
    ASSERT_EQUAL(smin(x, 3), 3);
    ASSERT_EQUAL(x, 3);
}

TEST("sqr") {
    ASSERT_EQUAL(sqr(0), 0);
    ASSERT_EQUAL(sqr(2), 4);
    ASSERT_EQUAL(sqr(-2), 4);
}
