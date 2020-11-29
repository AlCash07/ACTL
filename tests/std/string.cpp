/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/range/traits/all.hpp>
#include <actl/std/string.hpp>

#include "io/text/test_io.hpp"

TEST("category") {
    using C = std::string;
    ASSERT_TRUE(is_sequence_range_v<C>);
    ASSERT_TRUE(is_random_access_range_v<C>);
    ASSERT_TRUE(is_contiguous_range_v<C>);
    ASSERT_FALSE(is_associative_range_v<C>);
}

using namespace ac::io;

TEST("read") {
    std::string s = "  01234567890123456789 0123456789";
    std::string x;
    ASSERT_TRUE(read(memory<in>{s}, skipws{} >>= text{}, x));
    ASSERT_EQUAL("01234567890123456789"sv, x);
}

TEST("write") { test_write("abacaba", text{}, std::string{"abacaba"}); }
