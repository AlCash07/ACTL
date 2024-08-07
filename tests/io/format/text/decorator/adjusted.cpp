// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/device/memory.hpp>
#include <actl/io/format/text/arithmetic/integer_write.hpp>
#include <actl/io/format/text/decorator/adjusted.hpp>
#include <actl/io/format/text/text.hpp>
#include "test.hpp"

using namespace ac::io;

using pss = std::pair<size_t, size_t>;

TEST_CASE("adjustment::left") {
    adjusted fmt{4};
    CHECK(pss{0, 0} == adjustment(fmt, 8));
    CHECK(pss{0, 0} == adjustment(fmt, 4));
    CHECK(pss{0, 3} == adjustment(fmt, 1));
}

TEST_CASE("adjustment::right") {
    adjusted fmt{4, adjust_to::right};
    CHECK(pss{0, 0} == adjustment(fmt, 8));
    CHECK(pss{0, 0} == adjustment(fmt, 4));
    CHECK(pss{3, 0} == adjustment(fmt, 1));
}

TEST_CASE("adjustment::center") {
    adjusted fmt{4, adjust_to::center};
    CHECK(pss{0, 0} == adjustment(fmt, 8));
    CHECK(pss{0, 0} == adjustment(fmt, 4));
    CHECK(pss{1, 1} == adjustment(fmt, 2));
    CHECK(pss{1, 2} == adjustment(fmt, 1));
}

TEST_CASE("adjustment::center_right") {
    adjusted fmt{4, adjust_to::center_right};
    CHECK(pss{0, 0} == adjustment(fmt, 8));
    CHECK(pss{0, 0} == adjustment(fmt, 4));
    CHECK(pss{1, 1} == adjustment(fmt, 2));
    CHECK(pss{2, 1} == adjustment(fmt, 1));
}

TEST_CASE("fill") {
    auto fmt = text_static{} >>= adjusted{4, adjust_to::center, '*'};
    char s[14];
    memory<io::out> od{s};
    CHECK(write(od, fmt, 'a'));
    CHECK(write(od, fmt, "bacaba"));
    CHECK(write(od, fmt, 42));
    CHECK_EQUAL("*a**bacaba*42*"sv, s);
}
