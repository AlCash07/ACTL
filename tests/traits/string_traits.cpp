/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/traits/string_traits.hpp>
#include <actl/test.hpp>
#include <vector>

using namespace ac;

TEST("is_string") {
    ASSERT_TRUE(is_string_v<char[]>);
    ASSERT_TRUE(is_string_v<char[9]>);
    ASSERT_TRUE(is_string_v<char*>);
    ASSERT_TRUE(is_string_v<const char*>);
    ASSERT_TRUE(is_string_v<std::string>);
    ASSERT_FALSE(is_string_v<char>);
    ASSERT_FALSE(is_string_v<std::vector<char>>);
}
