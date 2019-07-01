/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/test.hpp>
#include <actl/util/type_traits.hpp>

TEST("add_const_if") {
    ASSERT_TRUE(std::is_same_v<int, add_const_if_t<false, int>>);
    ASSERT_TRUE(std::is_same_v<const int, add_const_if_t<true, int>>);
}

TEST("remove_cvref") { ASSERT_TRUE(std::is_same_v<int, remove_cvref_t<volatile const int&>>); }

TEST("nth_type") {
    ASSERT_TRUE(std::is_same_v<int, nth_t<0, int, float>>);
    ASSERT_TRUE(std::is_same_v<float, nth_t<1, int, float>>);
}
