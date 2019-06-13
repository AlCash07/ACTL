/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/test.hpp>
#include <actl/traits/type_traits.hpp>

TEST("add_const_if") {
    ASSERT_TRUE(std::is_same_v<int, add_const_if_t<false, int>>);
    ASSERT_TRUE(std::is_same_v<const int, add_const_if_t<true, int>>);
}

TEST("remove_cvref") { ASSERT_TRUE(std::is_same_v<int, remove_cvref_t<volatile const int&>>); }

TEST("remove_rvalue_ref") {
    ASSERT_TRUE(std::is_same_v<int, remove_rvalue_ref_t<int>>);
    ASSERT_TRUE(std::is_same_v<int&, remove_rvalue_ref_t<int&>>);
    ASSERT_TRUE(std::is_same_v<int, remove_rvalue_ref_t<int&&>>);
}
