/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/text/boolean.hpp>
#include "test_output.hpp"

using namespace ac::io;

TEST("write") {
    text_format<out> f;
    test_output("true", f, boolalpha, true);
    test_output("false", f, false);
    test_output("1", f, noboolalpha, true);
    test_output("0", f, false);
}

TEST("read") {
    text_format<out> f;
    bool x;
    ASSERT_TRUE(read(string<in>{"true"}, f, boolalpha, x));
    ASSERT_EQUAL(true, x);
    ASSERT_TRUE(read(string<in>{"false"}, f, x));
    ASSERT_EQUAL(false, x);
    ASSERT_FALSE(read(string<in>{"fals"}, f, x));
    ASSERT_FALSE(read(string<in>{"truth"}, f, x));
    ASSERT_FALSE(read(string<in>{"no"}, f, x));
    ASSERT_TRUE(read(string<in>{"1"}, f, noboolalpha, x));
    ASSERT_EQUAL(true, x);
    ASSERT_TRUE(read(string<in>{"0"}, f, x));
    ASSERT_EQUAL(false, x);
    ASSERT_FALSE(read(string<in>{"true"}, f, x));
}
