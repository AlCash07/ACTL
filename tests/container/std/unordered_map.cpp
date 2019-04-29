/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/std/unordered_map.hpp>
#include <actl/test.hpp>

TEST("category") {
    using C = std::unordered_map<int, int>;
    ASSERT_FALSE(is_sequence_container_v<C>);
    ASSERT_TRUE(is_associative_container_v<C>);
    ASSERT_FALSE(is_sorted_associative_container_v<C>);
    ASSERT_TRUE(is_unique_associative_container_v<C>);
    ASSERT_FALSE(is_multiple_associative_container_v<C>);
    ASSERT_FALSE(is_simple_associative_container_v<C>);
    ASSERT_TRUE(is_pair_associative_container_v<C>);
}

TEST("category") {
    using C = std::unordered_multimap<int, int>;
    ASSERT_FALSE(is_sequence_container_v<C>);
    ASSERT_TRUE(is_associative_container_v<C>);
    ASSERT_FALSE(is_sorted_associative_container_v<C>);
    ASSERT_FALSE(is_unique_associative_container_v<C>);
    ASSERT_TRUE(is_multiple_associative_container_v<C>);
    ASSERT_FALSE(is_simple_associative_container_v<C>);
    ASSERT_TRUE(is_pair_associative_container_v<C>);
}
