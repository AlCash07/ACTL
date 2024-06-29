// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/string/traits.hpp>
#include "test.hpp"

TEST_CASE("is_string") {
    CHECK(is_string_v<char[4]>);
    CHECK_FALSE(is_string_v<char16_t[4]>);
    CHECK(is_string_v<char16_t[4], char16_t>);
    CHECK(is_string_v<std::string>);
    CHECK(is_string_v<std::basic_string<char32_t>, char32_t>);
    CHECK_FALSE(is_string_v<char>);
    CHECK_FALSE(is_string_v<std::vector<char>>);
}
