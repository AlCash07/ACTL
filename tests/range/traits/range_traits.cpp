/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/range/traits/range_traits.hpp>
#include <actl/test.hpp>

TEST("is_container") {
    ASSERT_FALSE(is_container_v<int>);
    ASSERT_FALSE(is_container_v<int*>);
    ASSERT_FALSE(is_container_v<int[]>);
    ASSERT_TRUE(is_container_v<int[2]>);
}
