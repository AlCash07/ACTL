/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/device/memory.hpp>
#include <actl/std/tuple.hpp>
#include <actl/test.hpp>

using namespace ac::io;

TEST("write") {
    char s[5];
    ASSERT_TRUE(3 <=
                write(memory<bin | io::out>{s}, std::tuple{'a', 'c', 'a'}, std::tuple{'b', 'a', 'c'}));
    ASSERT_EQUAL("aca"sv, span{s, 3});
}

TEST("read") {
    std::string s = "abaca";
    memory<bin | in> id{s};
    std::tuple<char, char, char> x;
    ASSERT_TRUE(read(id, x));
    ASSERT_EQUAL(std::tuple{'a', 'b', 'a'}, x);
    ASSERT_FALSE(read(id, x));
}
