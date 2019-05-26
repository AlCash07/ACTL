/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/algorithm/binary_pow.hpp>
#include <actl/test.hpp>

TEST("correctness") {
    ASSERT_EQUAL(1, binary_pow(1024, 0));
    ASSERT_EQUAL(1 << 15, binary_pow(2, 15));
    ASSERT_EQUAL(1 << 16, binary_pow(2, 16));
    ASSERT_EQUAL(1ll << 62, binary_pow(2ll, 62));
}
