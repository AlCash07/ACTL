// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/std/set.hpp>
#include <actl/test.hpp>

TEST("set::category") {
    using C = std::set<int>;
    ASSERT_FALSE(is_sequence_range_v<C>);
    ASSERT_TRUE(is_associative_range_v<C>);
    ASSERT_TRUE(is_sorted_range_v<C>);
    ASSERT_TRUE(is_unique_range_v<C>);
    ASSERT_TRUE(is_simple_associative_range_v<C>);
    ASSERT_FALSE(is_pair_associative_range_v<C>);
}

TEST("multiset::category") {
    using C = std::multiset<int>;
    ASSERT_FALSE(is_sequence_range_v<C>);
    ASSERT_TRUE(is_associative_range_v<C>);
    ASSERT_TRUE(is_sorted_range_v<C>);
    ASSERT_FALSE(is_unique_range_v<C>);
    ASSERT_TRUE(is_simple_associative_range_v<C>);
    ASSERT_FALSE(is_pair_associative_range_v<C>);
}
