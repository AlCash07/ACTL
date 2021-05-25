// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/numeric/bit.hpp>
#include "test.hpp"

constexpr auto two62 = 4611686018427387904ll;

TEST_CASE("bit")
{
    CHECK(1 == bit(0));
    CHECK(two62 == bit(62ll));
    CHECK(two62 == bit<long long>(62));
}

TEST_CASE("has_bit")
{
    CHECK(has_bit(two62, 62));
    CHECK_FALSE(has_bit(two62, 61));
}
