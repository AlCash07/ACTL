/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/device/memory.hpp>
#include <actl/std/utility.hpp>
#include <actl/test.hpp>

using namespace ac::io;

TEST("write pair") {
    char s[3];
    ASSERT_TRUE(2 <= write(memory<bin | out>{s}, std::pair{'a', 'c'}, std::pair{'b', 'c'}));
    ASSERT_EQUAL_RANGES("ac"s, span{s, 2});
}

TEST("read pair") {
    std::string s = "aba";
    memory<bin | in> id{s};
    std::pair<char, char> x;
    ASSERT_TRUE(read(id, x));
    ASSERT_EQUAL(std::pair{'a', 'b'}, x);
    ASSERT_FALSE(read(id, x));
}
