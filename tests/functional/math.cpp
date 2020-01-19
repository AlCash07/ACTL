/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/functional/math.hpp>
#include <actl/test.hpp>
#include <limits>

TEST("abs") {
    ASSERT_EQUAL(0, op::abs(0));
    constexpr auto int_max = std::numeric_limits<int>::max();
    ASSERT_EQUAL(int_max, op::abs(int_max));
    ASSERT_EQUAL(int_max, op::abs(-int_max));
    constexpr auto uint_max = std::numeric_limits<unsigned int>::max();
    ASSERT_EQUAL(uint_max, op::abs(uint_max));
}
