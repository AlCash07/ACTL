// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/string/traits.hpp>
#include <actl/test.hpp>

TEST("is_string") {
    ASSERT_TRUE(is_string_v<char[4]>);
    ASSERT_FALSE(is_string_v<char16_t[4]>);
    ASSERT_TRUE(is_string_v<char16_t[4], char16_t>);
    ASSERT_TRUE(is_string_v<std::string>);
    ASSERT_TRUE(is_string_v<std::basic_string<char32_t>, char32_t>);
    ASSERT_FALSE(is_string_v<char>);
    ASSERT_FALSE(is_string_v<std::vector<char>>);
}
