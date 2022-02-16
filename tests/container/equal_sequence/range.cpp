// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/equal_sequence/range.hpp>
#include "test.hpp"

TEST_CASE("equal_sequence for vectors")
{
    CHECK(ac::equal_sequence(
        std::vector<int>{4, 2}, std::vector<long long>{4, 2}));
    /* size mismatch with a common prefix */ {
        CHECK(!ac::equal_sequence(std::vector{4}, std::vector{4, 2}));
        CHECK(!ac::equal_sequence(std::vector{4, 2}, std::vector{4}));
    }
    /* element mismatch */ {
        CHECK(!ac::equal_sequence(std::vector{4, 4}, std::vector{4, 2}));
        CHECK(!ac::equal_sequence(std::vector{5}, std::vector{4, 2}));
        CHECK(!ac::equal_sequence(std::vector{5, 2}, std::vector{4}));
    }
}
