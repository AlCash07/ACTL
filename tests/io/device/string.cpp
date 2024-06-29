// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/device/string.hpp>
#include "test.hpp"

TEST_CASE("input") {
    std::string s = "abcde";
    io::string<io::in> id{s};
    CHECK('a' == id.get());
    CHECK('b' == id.get());
    id.move(-1);
    char dst[9];
    CHECK(4ul == id.read({dst, 5}));
    CHECK("bcde"sv == std::string(dst, dst + 4));
    s += 'f';
    CHECK(1ul == id.read({dst, 3}));
    CHECK("f"sv == std::string(dst, dst + 1));
    CHECK(0ul == id.read({dst, 3}));
    CHECK('\0' == id.get());
}

TEST_CASE("output") {
    std::string s;
    io::string<io::app> od{s};
    CHECK(1ul == od.write('a'));
    CHECK(3ul == od.write({"bcd", 3}));
    CHECK("abcd"sv == s);
}
