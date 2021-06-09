// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/device/memory.hpp>
#include <actl/std/tuple.hpp>
#include "test.hpp"

using namespace ac::io;

TEST_CASE("tuple write")
{
    char s[5];
    CHECK(!write(
        memory<bin | io::out>{s},
        std::tuple{'a', 'c', 'a'},
        std::tuple{'b', 'a', 'c'}));
    CHECK_EQUAL("aca"sv, span{s, 3});
}

TEST_CASE("tuple read")
{
    std::string s = "abaca";
    memory<bin | in> id{s};
    std::tuple<char, char, char> x;
    CHECK(read(id, x));
    CHECK(std::tuple{'a', 'b', 'a'} == x);
    CHECK_FALSE(read(id, x));
}
