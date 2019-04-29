/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/std/vector.hpp>
#include <actl/test.hpp>
#include <actl/traits/container_traits.hpp>

TEST("array::category") {
    using C = int[2];
    ASSERT_TRUE(is_sequence_container_v<C>);
    ASSERT_TRUE(is_random_access_container_v<C>);
    ASSERT_FALSE(is_associative_container_v<C>);
}

TEST("is_container") {
    ASSERT_FALSE(is_container_v<int>);
    ASSERT_FALSE(is_container_v<int*>);
    ASSERT_FALSE(is_container_v<int[]>);
    ASSERT_TRUE(is_container_v<int[2]>);
}

TEST("rebind_container") {
    ASSERT_TRUE(std::is_same_v<dummy_container, rebind_container_t<none, int>>);
    ASSERT_TRUE(std::is_same_v<dummy_container, rebind_container_t<dummy_container, int>>);
    ASSERT_TRUE(std::is_same_v<dummy_container, rebind_container_t<std::vector<int>, none>>);
    ASSERT_TRUE(std::is_same_v<std::vector<int>, rebind_container_t<std::vector<none>, int>>);
}
