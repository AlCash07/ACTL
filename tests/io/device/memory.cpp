/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/device/memory.hpp>
#include <actl/test.hpp>

using namespace ac;

TEST("input") {
    const char s[] = "abcdef";
    io::memory<io::in> in{{s, s + 5}};
    ASSERT_EQUAL('a', in.get());
    ASSERT_EQUAL('b', in.get());
    in.move(-1);
    char dst[9];
    ASSERT_EQUAL(3, in.read({dst, 3}));
    ASSERT_EQUAL("bcd", std::string(dst, dst + 3));
    ASSERT_EQUAL(1, in.read({dst, 3}));
    ASSERT_EQUAL("e", std::string(dst, dst + 1));
    ASSERT_EQUAL(0, in.read({dst, 3}));
    ASSERT_EQUAL('\0', in.get());
}

TEST("output") {
    char s[5];
    io::memory<io::out> out{s};
    ASSERT_TRUE(out.put('a'));
    ASSERT_EQUAL(3, out.write({"bcd", 3}));
    ASSERT_EQUAL(1, out.write({"ef", 2}));
    ASSERT_FALSE(out.put('g'));
    ASSERT_EQUAL("abcde", std::string(s, s + 5));
}
