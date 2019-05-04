/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/device/memory.hpp>
#include <actl/io/text/text_format.hpp>
#include <actl/test.hpp>

using namespace ac::io;

using pi = std::pair<ac::index, ac::index>;

TEST("adjustment::default") {
    text_static<char, 0, 0, 0, 4> ts;
    ASSERT_EQUAL(pi{0, 0}, adjustment(ts, 8));
    ASSERT_EQUAL(pi{0, 0}, adjustment(ts, 4));
    ASSERT_EQUAL(pi{0, 3}, adjustment(ts, 1));
}

TEST("adjustment::right") {
    text_static<char, bits(flags::right), 0, 0, 4> ts;
    ASSERT_EQUAL(pi{0, 0}, adjustment(ts, 8));
    ASSERT_EQUAL(pi{0, 0}, adjustment(ts, 4));
    ASSERT_EQUAL(pi{0, 3}, adjustment(ts, 1));
}

TEST("adjustment::left") {
    text_static<char, bits(flags::left), 0, 0, 4> ts;
    ASSERT_EQUAL(pi{0, 0}, adjustment(ts, 8));
    ASSERT_EQUAL(pi{0, 0}, adjustment(ts, 4));
    ASSERT_EQUAL(pi{3, 0}, adjustment(ts, 1));
}

TEST("adjustment::center") {
    text_static<char, bits(flags::center), 0, 0, 4> ts;
    ASSERT_EQUAL(pi{0, 0}, adjustment(ts, 8));
    ASSERT_EQUAL(pi{0, 0}, adjustment(ts, 4));
    ASSERT_EQUAL(pi{1, 1}, adjustment(ts, 2));
    ASSERT_EQUAL(pi{1, 2}, adjustment(ts, 1));
}

TEST("adjustment::centerleft") {
    text_static<char, bits(flags::center) | bits(flags::left), 0, 0, 4> ts;
    ASSERT_EQUAL(pi{0, 0}, adjustment(ts, 8));
    ASSERT_EQUAL(pi{0, 0}, adjustment(ts, 4));
    ASSERT_EQUAL(pi{1, 1}, adjustment(ts, 2));
    ASSERT_EQUAL(pi{2, 1}, adjustment(ts, 1));
}

TEST("fill") {
    text_static<char, bits(flags::center), 0, 0, 4, '*'> ts;
    char s[10];
    io::memory<io::bin | io::out> od{s};
    ASSERT_EQUAL(4, write(od, ts, 'a'));
    ASSERT_EQUAL(6, write(od, ts, "bacaba"));
    ASSERT_EQUAL_RANGES("*a**bacaba"s, s);
}
