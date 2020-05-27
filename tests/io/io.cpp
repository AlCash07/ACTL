/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/device/memory.hpp>
#include <actl/io/text/text.hpp>
#include <actl/test.hpp>

using namespace ac::io;

TEST("write char") {
    char s[2];
    ASSERT_EQUAL(2l, write(memory<bin | io::out>{s}, 'a', 'b', 'c'));
    ASSERT_EQUAL("ab"sv, s);
}

TEST("read char") {
    std::string s = "ab";
    memory<bin | in> id{s};
    char a, b;
    ASSERT_TRUE(read(id, a, b));
    ASSERT_EQUAL('a', a);
    ASSERT_EQUAL('b', b);
    ASSERT_FALSE(read(id, a));
}

TEST("write char array") {
    char s[7];
    ASSERT_EQUAL(7l, write(memory<io::out>{s}, "aba", "cabad"));
    ASSERT_EQUAL("abacaba"sv, s);
}

TEST("read span<char>") {
    std::string s = "abacaba";
    memory<bin | in> id{s};
    char a[3], b[3];
    ASSERT_TRUE(read(id, span{a}, span{b}));
    ASSERT_EQUAL("aba"sv, a);
    ASSERT_EQUAL("cab"sv, b);
    ASSERT_FALSE(read(id, span{a}));
}

TEST("read string_view") {
    std::string s = "abacaba";
    memory<in> id{s};
    ASSERT_TRUE(read(id, "aba"sv));
    ASSERT_FALSE(read(id, "caca"sv));
}
