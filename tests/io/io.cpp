// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/argument/const_byte_span.hpp>
#include <actl/io/device/memory.hpp>
#include <actl/io/format/text/text.hpp>
#include "test.hpp"

using namespace ac::io;

TEST_CASE("write char") {
    char s[2];
    CHECK(!write(memory<bin | io::out>{s}, 'a', 'b', 'c'));
    CHECK_EQUAL("ab"sv, s);
}

TEST_CASE("read char") {
    std::string s = "ab";
    memory<bin | in> id{s};
    char a, b;
    CHECK(read(id, a, b));
    CHECK('a' == a);
    CHECK('b' == b);
    CHECK_FALSE(read(id, a));
}

TEST_CASE("write char array") {
    char s[7];
    CHECK(!write(memory<io::out>{s}, "aba", "cabad"));
    CHECK_EQUAL("abacaba"sv, s);
}

TEST_CASE("read span<char>") {
    std::string s = "abacaba";
    memory<bin | in> id{s};
    char a[3], b[3];
    CHECK(read(id, span{a}, span{b}));
    CHECK_EQUAL("aba"sv, a);
    CHECK_EQUAL("cab"sv, b);
    CHECK_FALSE(read(id, span{a}));
}

TEST_CASE("read string_view") {
    std::string s = "abacaba";
    memory<in> id{s};
    CHECK(read(id, "aba"sv));
    CHECK_FALSE(read(id, "caca"sv));
}
