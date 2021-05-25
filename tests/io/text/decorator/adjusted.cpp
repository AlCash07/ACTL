// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/device/memory.hpp>
#include <actl/io/text/arithmetic/integer_write.hpp>
#include <actl/io/text/decorator/adjusted.hpp>
#include <actl/io/text/text.hpp>
#include "test.hpp"

using namespace ac::io;

using pii = std::pair<ac::index, ac::index>;

TEST_CASE("adjustment::left")
{
    adjusted fmt{4};
    CHECK(pii{0, 0} == adjustment(fmt, 8));
    CHECK(pii{0, 0} == adjustment(fmt, 4));
    CHECK(pii{0, 3} == adjustment(fmt, 1));
}

TEST_CASE("adjustment::right")
{
    adjusted fmt{4, adjust_to::right};
    CHECK(pii{0, 0} == adjustment(fmt, 8));
    CHECK(pii{0, 0} == adjustment(fmt, 4));
    CHECK(pii{3, 0} == adjustment(fmt, 1));
}

TEST_CASE("adjustment::center")
{
    adjusted fmt{4, adjust_to::center};
    CHECK(pii{0, 0} == adjustment(fmt, 8));
    CHECK(pii{0, 0} == adjustment(fmt, 4));
    CHECK(pii{1, 1} == adjustment(fmt, 2));
    CHECK(pii{1, 2} == adjustment(fmt, 1));
}

TEST_CASE("adjustment::center_right")
{
    adjusted fmt{4, adjust_to::center_right};
    CHECK(pii{0, 0} == adjustment(fmt, 8));
    CHECK(pii{0, 0} == adjustment(fmt, 4));
    CHECK(pii{1, 1} == adjustment(fmt, 2));
    CHECK(pii{2, 1} == adjustment(fmt, 1));
}

TEST_CASE("fill")
{
    auto fmt = text_static{} >>= adjusted{4, adjust_to::center, '*'};
    char s[14];
    memory<io::out> od{s};
    CHECK(4l == write(od, fmt, 'a'));
    CHECK(6l == write(od, fmt, "bacaba"));
    CHECK(4l == write(od, fmt, 42));
    CHECK_EQUAL("*a**bacaba*42*"sv, s);
}
