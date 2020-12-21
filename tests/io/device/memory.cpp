// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/io/device/memory.hpp>

TEST_CASE("memory input") {
    const char s[] = "abcdef";
    io::memory<io::in> id{span{s, s + 5}};
    CHECK('a' == id.get());
    CHECK('b' == id.get());
    id.move(-1);
    char dst[9];
    CHECK(3l == id.read({dst, 3}));
    CHECK("bcd"sv == std::string(dst, dst + 3));
    CHECK(1l == id.read({dst, 3}));
    CHECK("e"sv == std::string(dst, dst + 1));
    CHECK(0l == id.read({dst, 3}));
    CHECK('\0' == id.get());
}

TEST_CASE("memory output") {
    char s[5];
    io::memory<io::out> od{s};
    CHECK(1l == od.write('a'));
    CHECK(3l == od.write({"bcd", 3}));
    CHECK(1l == od.write({"ef", 2}));
    CHECK(0l == od.write('g'));
    CHECK("abcde"sv == std::string(s, s + 5));
}
