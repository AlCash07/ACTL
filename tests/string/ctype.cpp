/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/string/ctype.hpp>
#include <actl/test.hpp>

TEST("correctness") {
    for (char c = std::numeric_limits<char>::min();; ++c) {
        auto uc = static_cast<unsigned char>(c);
        ASSERT_EQUAL(isdigit(uc) != 0, is_digit(c));
        ASSERT_EQUAL(islower(uc) != 0, is_lower(c));
        ASSERT_EQUAL(isupper(uc) != 0, is_upper(c));
        ASSERT_EQUAL(isspace(uc) != 0, is_space(c));
        if (c == std::numeric_limits<char>::max()) break;
    }
}
