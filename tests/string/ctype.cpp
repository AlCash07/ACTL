// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/string/ctype.hpp>
#include <cctype>
#include "test.hpp"

TEST_CASE("ctype correctness")
{
    for (char c = std::numeric_limits<char>::min();; ++c)
    {
        auto uc = static_cast<unsigned char>(c);
        CHECK((std::isdigit(uc) != 0) == ac::is_digit(c));
        CHECK((std::islower(uc) != 0) == ac::is_lower(c));
        CHECK((std::isupper(uc) != 0) == ac::is_upper(c));
        CHECK((std::isalpha(uc) != 0) == ac::is_alpha(c));
        CHECK((std::isalnum(uc) != 0) == ac::is_alphanumeric(c));
        CHECK((std::isspace(uc) != 0) == ac::is_space(c));
        if (c == std::numeric_limits<char>::max())
            break;
    }
}
