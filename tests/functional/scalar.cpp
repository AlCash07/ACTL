/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/functional/scalar.hpp>
#include <actl/test.hpp>
#include <limits>

TEST("inplace parameter") {
    int x = 4;
    ASSERT_EQUAL(5, op::max(5, inplace(x)));
    ASSERT_EQUAL(5, x);
    ASSERT_EQUAL(5, op::max(inplace(x), 3));
    ASSERT_EQUAL(5, x);
}

TEST("output parameter") {
    int res{};
    ASSERT_EQUAL(6, op::add(out(res), 2, 4));
    ASSERT_EQUAL(6, res);
}

TEST("arithmetics") {
    ASSERT_EQUAL(-6, op::neg(6));
    ASSERT_EQUAL(2, op::neg(-2));
    ASSERT_EQUAL(8, op::add(6, 2));
    ASSERT_EQUAL(3, op::div(6, 2));
    ASSERT_EQUAL(12, op::mul(6, 2));
    ASSERT_EQUAL(4, op::sub(6, 2));
}

TEST("op::cmp3way") {
    ASSERT_EQUAL(1, op::cmp3way(0, -1));
    ASSERT_EQUAL(-1, op::cmp3way(0u, 1u));
}

TEST("logical") {
    ASSERT_EQUAL(true, op::logical_not(false));
    ASSERT_EQUAL(false, op::logical_not(true));
    ASSERT_EQUAL(true, op::logical_and(true, true));
    ASSERT_EQUAL(false, op::logical_and(true, false));
    ASSERT_EQUAL(true, op::logical_or(true, false));
    ASSERT_EQUAL(false, op::logical_or(false, false));
}

TEST("bit") {
    ASSERT_EQUAL(-1, op::bit_not(0));
    ASSERT_EQUAL(1, op::bit_and(3, 5));
    ASSERT_EQUAL(7, op::bit_or(3, 5));
}

TEST("op::sgn") {
    ASSERT_EQUAL(1, op::sgn(std::numeric_limits<int>::max()));
    ASSERT_EQUAL(-1, op::sgn(std::numeric_limits<int>::min()));
    ASSERT_EQUAL(1, op::sgn(std::numeric_limits<unsigned int>::max()));
    ASSERT_EQUAL(0, op::sgn(0));
}

TEST("op::sqr") {
    ASSERT_EQUAL(36, op::sqr(6));
    ASSERT_EQUAL(4, op::sqr(-2));
}
