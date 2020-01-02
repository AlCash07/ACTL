/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/functional/arithmetic.hpp>
#include <actl/test.hpp>

TEST("same type") {
    ASSERT_EQUAL(-6, op::neg(6));
    ASSERT_EQUAL(2, op::neg(-2));
    ASSERT_EQUAL(36, op::sqr(6));
    ASSERT_EQUAL(4, op::sqr(-2));
    ASSERT_EQUAL(8, op::add(6, 2));
    ASSERT_EQUAL(3, op::div(6, 2));
    ASSERT_EQUAL(12, op::mul(6, 2));
    ASSERT_EQUAL(4, op::sub(6, 2));
}
