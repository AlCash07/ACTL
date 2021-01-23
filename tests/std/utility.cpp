// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/device/memory.hpp>
#include <actl/std/utility.hpp>
#include "test.hpp"

using namespace ac::io;

TEST_CASE("write pair") {
    char s[3];
    CHECK(2 <= write(memory<bin | io::out>{s}, std::pair{'a', 'c'}, std::pair{'b', 'c'}));
    CHECK_EQUAL("ac"sv, span{s, 2});
}

TEST_CASE("read pair") {
    std::string s = "aba";
    memory<bin | in> id{s};
    std::pair<char, char> x;
    CHECK(read(id, x));
    CHECK(std::pair{'a', 'b'} == x);
    CHECK_FALSE(read(id, x));
}
