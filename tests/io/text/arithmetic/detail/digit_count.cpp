// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/io/text/arithmetic/detail/digit_count.hpp>
#include <actl/test.hpp>

TEST("digit_count") {
    using io::detail::digit_count;
    ASSERT_EQUAL(2l, digit_count(8, 3));
    ASSERT_EQUAL(9l, digit_count(999'999'999, 10));
    ASSERT_EQUAL(10l, digit_count(1'000'000'000, 10));
    ASSERT_EQUAL(64l, digit_count(std::numeric_limits<unsigned long long>::max(), 2ull));
}
