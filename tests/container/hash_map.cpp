/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/hash_map.hpp>
#include <actl/test.hpp>

TEST("hash_map::category") {
    using C = hash_map<int, int>;
    ASSERT_FALSE(is_sequence_container_v<C>);
    ASSERT_TRUE(is_associative_container_v<C>);
    ASSERT_FALSE(is_sorted_range_v<C>);
    ASSERT_TRUE(is_unique_range_v<C>);
    ASSERT_FALSE(is_simple_associative_container_v<C>);
    ASSERT_TRUE(is_pair_associative_container_v<C>);
}

TEST("hash_multimap::category") {
    using C = hash_multimap<int, int>;
    ASSERT_FALSE(is_sequence_container_v<C>);
    ASSERT_TRUE(is_associative_container_v<C>);
    ASSERT_FALSE(is_sorted_range_v<C>);
    ASSERT_FALSE(is_unique_range_v<C>);
    ASSERT_FALSE(is_simple_associative_container_v<C>);
    ASSERT_TRUE(is_pair_associative_container_v<C>);
}
