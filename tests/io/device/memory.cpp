// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/io/device/memory.hpp>
#include <actl/test.hpp>

TEST("input") {
    const char s[] = "abcdef";
    io::memory<io::in> id{span{s, s + 5}};
    ASSERT_EQUAL('a', id.get());
    ASSERT_EQUAL('b', id.get());
    id.move(-1);
    char dst[9];
    ASSERT_EQUAL(3l, id.read({dst, 3}));
    ASSERT_EQUAL("bcd"sv, std::string(dst, dst + 3));
    ASSERT_EQUAL(1l, id.read({dst, 3}));
    ASSERT_EQUAL("e"sv, std::string(dst, dst + 1));
    ASSERT_EQUAL(0l, id.read({dst, 3}));
    ASSERT_EQUAL('\0', id.get());
}

TEST("output") {
    char s[5];
    io::memory<io::out> od{s};
    ASSERT_EQUAL(1l, od.write('a'));
    ASSERT_EQUAL(3l, od.write({"bcd", 3}));
    ASSERT_EQUAL(1l, od.write({"ef", 2}));
    ASSERT_EQUAL(0l, od.write('g'));
    ASSERT_EQUAL("abcde"sv, std::string(s, s + 5));
}
