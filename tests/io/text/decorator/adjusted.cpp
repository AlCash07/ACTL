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

TEST("adjustment::left") {
    adjusted fmt;
    fmt.width = 4;
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 4));
    ASSERT_EQUAL(pii{0, 3}, adjustment(fmt, 1));
}

TEST("adjustment::right") {
    adjusted fmt;
    fmt.where = adjust_to::right;
    fmt.width = 4;
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 4));
    ASSERT_EQUAL(pii{3, 0}, adjustment(fmt, 1));
}

TEST("adjustment::center") {
    adjusted fmt;
    fmt.where = adjust_to::center;
    fmt.width = 4;
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 4));
    ASSERT_EQUAL(pii{1, 1}, adjustment(fmt, 2));
    ASSERT_EQUAL(pii{1, 2}, adjustment(fmt, 1));
}

TEST("adjustment::center_right") {
    adjusted fmt;
    fmt.where = adjust_to::center_right;
    fmt.width = 4;
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 8));
    ASSERT_EQUAL(pii{0, 0}, adjustment(fmt, 4));
    ASSERT_EQUAL(pii{1, 1}, adjustment(fmt, 2));
    ASSERT_EQUAL(pii{2, 1}, adjustment(fmt, 1));
}

TEST("fill") {
    std::tuple<text_static<>, adjusted<>> fmt;
    char s[14];
    memory<io::out> od{s};
    write(od, fmt, adjust_to::center);
    ASSERT_EQUAL(4, write(od, fmt, setwidth{4}, setfill{'*'}, 'a'));
    ASSERT_EQUAL(6, write(od, fmt, "bacaba"));
    ASSERT_EQUAL(4, write(od, fmt, 42));
    ASSERT_EQUAL("*a**bacaba*42*"sv, s);
}
