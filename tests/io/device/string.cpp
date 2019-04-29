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
    io::string<io::in> in{s};
    ASSERT_EQUAL('a', in.get());
    ASSERT_EQUAL('b', in.get());
    in.move(-1);
    char dst[9];
    ASSERT_EQUAL(4, in.read({dst, 5}));
    ASSERT_EQUAL("bcde", std::string(dst, dst + 4));
    s += 'f';
    ASSERT_EQUAL(1, in.read({dst, 3}));
    ASSERT_EQUAL("f", std::string(dst, dst + 1));
    ASSERT_EQUAL(0, in.read({dst, 3}));
    ASSERT_EQUAL('\0', in.get());
}

TEST("output") {
    std::string s;
    io::string<io::app> out{s};
    ASSERT_TRUE(out.put('a'));
    ASSERT_EQUAL(3, out.write({"bcd", 3}));
    ASSERT_EQUAL("abcd", s);
}
