// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/std/map.hpp>

TEST_CASE("map::category") {
    using C = std::map<int, int>;
    CHECK_FALSE(is_sequence_range_v<C>);
    CHECK(is_associative_range_v<C>);
    CHECK(is_sorted_range_v<C>);
    CHECK(is_unique_range_v<C>);
    CHECK_FALSE(is_simple_associative_range_v<C>);
    CHECK(is_pair_associative_range_v<C>);
}

TEST_CASE("multimap::category") {
    using C = std::multimap<int, int>;
    CHECK_FALSE(is_sequence_range_v<C>);
    CHECK(is_associative_range_v<C>);
    CHECK(is_sorted_range_v<C>);
    CHECK_FALSE(is_unique_range_v<C>);
    CHECK_FALSE(is_simple_associative_range_v<C>);
    CHECK(is_pair_associative_range_v<C>);
}
