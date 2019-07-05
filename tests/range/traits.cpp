/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/range/traits.hpp>
#include <actl/std/list.hpp>
#include <actl/std/vector.hpp>
#include <actl/test.hpp>

TEST("is_range") {
    ASSERT_FALSE(is_range_v<int>);
    ASSERT_TRUE(is_range_v<int[4]>);
    ASSERT_TRUE(is_range_v<const int[4]>);
    ASSERT_TRUE(is_range_v<std::vector<int>>);
    ASSERT_TRUE(is_range_v<std::list<int>>);
}

TEST("is_contiguous_range") {
    ASSERT_FALSE(is_contiguous_range_v<int>);
    ASSERT_TRUE(is_contiguous_range_v<int[4]>);
    ASSERT_TRUE(is_contiguous_range_v<const int[4]>);
    ASSERT_TRUE(is_contiguous_range_v<std::vector<int>>);
    ASSERT_FALSE(is_contiguous_range_v<std::list<int>>);
}