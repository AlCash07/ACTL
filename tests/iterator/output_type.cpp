/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/iterator/output_type.hpp>
#include <actl/std/deque.hpp>
#include <actl/test.hpp>

TEST("correctness") {
    ASSERT_TRUE(std::is_same_v<int, output_type_t<int*>>);
    ASSERT_TRUE(std::is_same_v<int, output_type_t<std::ostream_iterator<int>>>);
    ASSERT_TRUE(std::is_same_v<int, output_type_t<std::back_insert_iterator<std::deque<int>>>>);
    ASSERT_TRUE(std::is_same_v<int, output_type_t<std::front_insert_iterator<std::deque<int>>>>);
    ASSERT_TRUE(std::is_same_v<int, output_type_t<std::insert_iterator<std::deque<int>>>>);
}
