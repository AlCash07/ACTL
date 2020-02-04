/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/device/memory.hpp>
#include <actl/io/text/text.hpp>
#include <actl/io/util/skip.hpp>
#include <actl/test.hpp>

using namespace ac::io;

TEST("is_string") {
    ASSERT_TRUE(is_string_v<char[4]>);
    ASSERT_TRUE(is_string_v<std::string>);
}

TEST("whitespace skip") {
    std::string s = " \n\ta\n";
    memory<in> id{s};
    char a, b;
    ASSERT_TRUE(read(id, std::tuple<skipws, text>{}, a));
    ASSERT_TRUE(read(id, text{}, b));
    ASSERT_EQUAL('a', a);
    ASSERT_EQUAL('\n', b);
}
