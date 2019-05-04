/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/device/memory.hpp>
#include <actl/io/io.hpp>
#include <actl/test.hpp>

using namespace ac;

TEST("write char") {
    char s[2];
    ASSERT_EQUAL(2, write(io::memory<io::bin | io::out>{s}, 'a', 'b', 'c'));
    ASSERT_EQUAL_RANGES("ab"s, s);
}

TEST("read char") {
    std::string s = "ab";
    io::memory<io::bin | io::in> in{s};
    char a, b;
    ASSERT_TRUE(read(in, a, b));
    ASSERT_EQUAL('a', a);
    ASSERT_EQUAL('b', b);
    ASSERT_FALSE(read(in, a));
}

TEST("write span<char>") {
    char s[7];
    ASSERT_EQUAL(7, write(io::memory<io::bin | io::out>{s}, "aba", "cabad"));
    ASSERT_EQUAL_RANGES("abacaba"s, s);
}

TEST("read span<char>") {
    std::string s = "abacaba";
    io::memory<io::bin | io::in> in{s};
    char a[3], b[3];
    ASSERT_TRUE(read(in, a, b));
    ASSERT_EQUAL_RANGES("aba"s, a);
    ASSERT_EQUAL_RANGES("cab"s, b);
    ASSERT_FALSE(read(in, a));
}
