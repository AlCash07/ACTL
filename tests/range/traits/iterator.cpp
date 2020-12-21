// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/range/traits/iterator.hpp>
#include <actl/std/list.hpp>
#include <actl/std/vector.hpp>

TEST_CASE("is_contiguous_range") {
    CHECK_FALSE(is_contiguous_range_v<int>);
    CHECK(is_contiguous_range_v<int[4]>);
    CHECK(is_contiguous_range_v<const int[4]>);
    CHECK(is_contiguous_range_v<std::vector<int>>);
    CHECK_FALSE(is_contiguous_range_v<std::list<int>>);
}

TEST_CASE("C array category") {
    using C = int[2];
    CHECK(is_sequence_range_v<C>);
    CHECK(is_random_access_range_v<C>);
    CHECK(is_contiguous_range_v<C>);
    CHECK_FALSE(is_associative_range_v<C>);
}
