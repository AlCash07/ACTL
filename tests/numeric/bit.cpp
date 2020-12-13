// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/numeric/bit.hpp>
#include <actl/test.hpp>

constexpr auto two62 = 4611686018427387904ll;

TEST("bit") {
    ASSERT_EQUAL(1, bit(0));
    ASSERT_EQUAL(two62, bit(62ll));
    ASSERT_EQUAL(two62, bit<long long>(62));
}

TEST("has_bit") {
    ASSERT_TRUE(has_bit(two62, 62));
    ASSERT_FALSE(has_bit(two62, 61));
}
