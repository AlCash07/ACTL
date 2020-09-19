/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/device/string.hpp>
#include <actl/test.hpp>

TEST("input") {
    std::string s = "abcde";
    io::string<io::in> id{s};
    ASSERT_EQUAL('a', id.get());
    ASSERT_EQUAL('b', id.get());
    id.move(-1);
    char dst[9];
    ASSERT_EQUAL(4l, id.read({dst, 5}));
    ASSERT_EQUAL("bcde"sv, std::string(dst, dst + 4));
    s += 'f';
    ASSERT_EQUAL(1l, id.read({dst, 3}));
    ASSERT_EQUAL("f"sv, std::string(dst, dst + 1));
    ASSERT_EQUAL(0l, id.read({dst, 3}));
    ASSERT_EQUAL('\0', id.get());
}

TEST("output") {
    std::string s;
    io::string<io::app> od{s};
    ASSERT_EQUAL(1l, od.write('a'));
    ASSERT_EQUAL(3l, od.write({"bcd", 3}));
    ASSERT_EQUAL("abcd"sv, s);
}
