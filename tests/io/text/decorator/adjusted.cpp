/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/device/memory.hpp>
#include <actl/io/text/decorator/adjusted.hpp>
#include <actl/io/text/text.hpp>
#include <actl/test.hpp>

using namespace ac::io;

using pii = std::pair<ac::index, ac::index>;

#if 0
TEST("adjustment::default") {
    std::tuple<adjusted<>, text_static<>> fmt;
    fmt.width(4);
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 4));
    ASSERT_EQUAL(pii{0, 3}, adjustment(fmt, 1));
}

TEST("adjustment::right") {
    std::tuple<adjusted<>, text_static<bit(flags::right)>> fmt;
    fmt.width(4);
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 4));
    ASSERT_EQUAL(pii{0, 3}, adjustment(fmt, 1));
}

TEST("adjustment::left") {
    std::tuple<adjusted<>, text_static<bit(flags::left)>> fmt;
    fmt.width(4);
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 4));
    ASSERT_EQUAL(pii{3, 0}, adjustment(fmt, 1));
}

TEST("adjustment::center") {
    std::tuple<adjusted<>, text_static<bit(flags::center)>> fmt;
    fmt.width(4);
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 4));
    ASSERT_EQUAL(pii{1, 1}, adjustment(fmt, 2));
    ASSERT_EQUAL(pii{1, 2}, adjustment(fmt, 1));
}

TEST("adjustment::centerleft") {
    std::tuple<adjusted<>, text_static<bit(flags::center) | bit(flags::left)>> fmt;
    fmt.width(4);
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 4));
    ASSERT_EQUAL(pii{1, 1}, adjustment(fmt, 2));
    ASSERT_EQUAL(pii{2, 1}, adjustment(fmt, 1));
}

TEST("fill") {
    std::tuple<adjusted<>, text_static<bit(flags::center)>> fmt;
    char s[10];
    memory<out> od{s};
    ASSERT_EQUAL(4, write(od, fmt, setwidth{4}, setfill{'*'}, 'a'));
    ASSERT_EQUAL(6, write(od, fmt, "bacaba"));
    ASSERT_EQUAL("*a**bacaba"sv, s);
}
#endif
