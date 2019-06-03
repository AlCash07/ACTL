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
    ASSERT_EQUAL(2, write(memory<bin | out>{s}, 'a', 'b', 'c'));
    ASSERT_EQUAL_RANGES("ab"s, s);
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

TEST("write span<char>") {
    char s[7];
    ASSERT_EQUAL(7, write(memory<bin | out>{s}, char_span{"aba"}, char_span{"cabad"}));
    ASSERT_EQUAL_RANGES("abacaba"s, s);
}

TEST("read span<char>") {
    std::string s = "abacaba";
    memory<bin | in> id{s};
    char a[3], b[3];
    ASSERT_TRUE(read(id, span{a}, span{b}));
    ASSERT_EQUAL_RANGES("aba"s, a);
    ASSERT_EQUAL_RANGES("cab"s, b);
    ASSERT_FALSE(read(id, span{a}));
}

TEST("read cspan<char>") {
    std::string s = "abacaba";
    memory<bin | in> id{s};
    ASSERT_TRUE(read(id, char_span{"aba"}));
    ASSERT_FALSE(read(id, char_span{"caca"}));
}
