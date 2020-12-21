// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/std/set.hpp>

TEST_CASE("set::category") {
    using C = std::set<int>;
    CHECK_FALSE(is_sequence_range_v<C>);
    CHECK(is_associative_range_v<C>);
    CHECK(is_sorted_range_v<C>);
    CHECK(is_unique_range_v<C>);
    CHECK(is_simple_associative_range_v<C>);
    CHECK_FALSE(is_pair_associative_range_v<C>);
}

TEST_CASE("multiset::category") {
    using C = std::multiset<int>;
    CHECK_FALSE(is_sequence_range_v<C>);
    CHECK(is_associative_range_v<C>);
    CHECK(is_sorted_range_v<C>);
    CHECK_FALSE(is_unique_range_v<C>);
    CHECK(is_simple_associative_range_v<C>);
    CHECK_FALSE(is_pair_associative_range_v<C>);
}
