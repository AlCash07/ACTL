/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/std/array.hpp>
#include <actl/test.hpp>

TEST("category") {
    using C = std::array<int, 2>;
    ASSERT_TRUE(is_sequence_container_v<C>);
    ASSERT_TRUE(is_random_access_container_v<C>);
    ASSERT_TRUE(is_contiguous_range_v<C>);
    ASSERT_FALSE(is_associative_container_v<C>);
}
