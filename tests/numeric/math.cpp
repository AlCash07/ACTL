// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/numeric/math.hpp>
#include <limits>
#include "test.hpp"

TEST_CASE("abs")
{
    CHECK(0 == ac::abs(0));
    constexpr auto int_max = std::numeric_limits<int>::max();
    CHECK(int_max == ac::abs(int_max));
    CHECK(int_max == ac::abs(-int_max));
    constexpr auto uint_max = std::numeric_limits<unsigned int>::max();
    CHECK(uint_max == ac::abs(uint_max));
}
