/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/test.hpp>
#include <actl/traits/ctype.hpp>

using namespace ac;

TEST("ctype::correctness") {
    for (char c = std::numeric_limits<char>::min(); c != std::numeric_limits<char>::max(); ++c) {
        ASSERT_EQUAL(isdigit(c), is_digit(c));
        ASSERT_EQUAL(islower(c), is_lower(c));
        ASSERT_EQUAL(isupper(c), is_upper(c));
        ASSERT_EQUAL(isspace(c), is_space(c));
    }
}
