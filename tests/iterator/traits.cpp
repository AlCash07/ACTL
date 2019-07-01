/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/iterator/traits.hpp>
#include <actl/std/forward_list.hpp>
#include <actl/std/list.hpp>
#include <actl/std/vector.hpp>
#include <actl/test.hpp>
#include <iterator>

TEST("is_iterator") {
    ASSERT_FALSE(is_iterator_v<int[4]>);
    ASSERT_TRUE(is_iterator_v<int*>);
    ASSERT_TRUE(is_iterator_v<std::vector<int>::iterator>);
}

TEST("is_const_iterator") {
    ASSERT_FALSE(is_const_iterator_v<int[4]>);
    ASSERT_FALSE(is_const_iterator_v<int*>);
    ASSERT_TRUE(is_const_iterator_v<std::vector<int>::const_iterator>);
}

TEST("is_non_const_iterator") {
    ASSERT_FALSE(is_non_const_iterator_v<int[4]>);
    ASSERT_FALSE(is_non_const_iterator_v<const int*>);
    ASSERT_TRUE(is_non_const_iterator_v<std::vector<int>::iterator>);
}

TEST("has_iterator_category") {
    ASSERT_FALSE(is_input_iterator_v<int[4]>);
    ASSERT_TRUE(is_input_iterator_v<std::istream_iterator<int>>);
    ASSERT_TRUE(is_output_iterator_v<std::ostream_iterator<int>>);
    ASSERT_TRUE(is_forward_iterator_v<std::forward_list<int>::iterator>);
    ASSERT_TRUE(is_bidirectional_iterator_v<std::list<int>::iterator>);
    ASSERT_TRUE(is_random_access_iterator_v<std::vector<int>::iterator>);
}

TEST("is_range") {
    ASSERT_FALSE(is_range_v<int>);
    ASSERT_TRUE(is_range_v<int[4]>);
    ASSERT_TRUE(is_range_v<int(&)[4]>);
    ASSERT_TRUE(is_range_v<std::vector<int>>);
}