/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/device/memory.hpp>
#include <actl/io/text/manip.hpp>
#include <actl/io/text/text_format.hpp>
#include <actl/io/util/formatted.hpp>
#include <actl/test.hpp>

using namespace ac::io;

using pii = std::pair<ac::index, ac::index>;

TEST("adjustment::default") {
    text_static<char, 0, 0, 0, 4> ts;
    ASSERT_EQUAL(pii{0, 0}, adjustment(ts, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(ts, 4));
    ASSERT_EQUAL(pii{0, 3}, adjustment(ts, 1));
}

TEST("adjustment::right") {
    text_static<char, bits(flags::right), 0, 0, 4> ts;
    ASSERT_EQUAL(pii{0, 0}, adjustment(ts, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(ts, 4));
    ASSERT_EQUAL(pii{0, 3}, adjustment(ts, 1));
}

TEST("adjustment::left") {
    text_static<char, bits(flags::left), 0, 0, 4> ts;
    ASSERT_EQUAL(pii{0, 0}, adjustment(ts, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(ts, 4));
    ASSERT_EQUAL(pii{3, 0}, adjustment(ts, 1));
}

TEST("adjustment::center") {
    text_static<char, bits(flags::center), 0, 0, 4> ts;
    ASSERT_EQUAL(pii{0, 0}, adjustment(ts, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(ts, 4));
    ASSERT_EQUAL(pii{1, 1}, adjustment(ts, 2));
    ASSERT_EQUAL(pii{1, 2}, adjustment(ts, 1));
}

TEST("adjustment::centerleft") {
    text_static<char, bits(flags::center) | bits(flags::left), 0, 0, 4> ts;
    ASSERT_EQUAL(pii{0, 0}, adjustment(ts, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(ts, 4));
    ASSERT_EQUAL(pii{1, 1}, adjustment(ts, 2));
    ASSERT_EQUAL(pii{2, 1}, adjustment(ts, 1));
}

TEST("fill") {
    text_static<char, bits(flags::center), 0, 0, 4, '*'> ts;
    char s[10];
    memory<out> od{s};
    ASSERT_EQUAL(4, write(od, ts, 'a'));
    ASSERT_EQUAL(6, write(od, ts, "bacaba"));
    ASSERT_EQUAL_RANGES("*a**bacaba"s, s);
}

TEST("read char") {
    std::string s = " \n\ta\n";
    formatted<memory<in>> id{s};
    char a, b;
    ASSERT_TRUE(read(id, skipws, a, noskipws, b));
    ASSERT_EQUAL('a', a);
    ASSERT_EQUAL('\n', b);
}
