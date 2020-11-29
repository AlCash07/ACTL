/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/range/traits/iterator.hpp>
#include <actl/std/list.hpp>
#include <actl/std/vector.hpp>
#include <actl/test.hpp>

TEST("is_contiguous_range") {
    ASSERT_FALSE(is_contiguous_range_v<int>);
    ASSERT_TRUE(is_contiguous_range_v<int[4]>);
    ASSERT_TRUE(is_contiguous_range_v<const int[4]>);
    ASSERT_TRUE(is_contiguous_range_v<std::vector<int>>);
    ASSERT_FALSE(is_contiguous_range_v<std::list<int>>);
}

TEST("C array category") {
    using C = int[2];
    ASSERT_TRUE(is_sequence_range_v<C>);
    ASSERT_TRUE(is_random_access_range_v<C>);
    ASSERT_TRUE(is_contiguous_range_v<C>);
    ASSERT_FALSE(is_associative_range_v<C>);
}
