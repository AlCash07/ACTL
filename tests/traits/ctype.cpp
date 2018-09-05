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
        auto uc = static_cast<unsigned char>(c);
        ASSERT_EQUAL(isdigit(uc) != 0, is_digit(c));
        ASSERT_EQUAL(islower(uc) != 0, is_lower(c));
        ASSERT_EQUAL(isupper(uc) != 0, is_upper(c));
        ASSERT_EQUAL(isspace(uc) != 0, is_space(c));
    }
}