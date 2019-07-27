/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/functional/arithmetic.hpp>
#include <actl/test.hpp>

TEST("sqr") {
    ASSERT_EQUAL(0, sqr(0));
    ASSERT_EQUAL(4, sqr(2));
    ASSERT_EQUAL(4, sqr(-2));
}
