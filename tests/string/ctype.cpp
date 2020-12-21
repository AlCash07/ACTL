// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/string/ctype.hpp>

TEST_CASE("ctype correctness") {
    for (char c = std::numeric_limits<char>::min();; ++c) {
        auto uc = static_cast<unsigned char>(c);
        CHECK((isdigit(uc) != 0) == is_digit(c));
        CHECK((islower(uc) != 0) == is_lower(c));
        CHECK((isupper(uc) != 0) == is_upper(c));
        CHECK((isspace(uc) != 0) == is_space(c));
        if (c == std::numeric_limits<char>::max()) break;
    }
}
