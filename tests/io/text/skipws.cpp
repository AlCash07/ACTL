// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/io/device/memory.hpp>
#include <actl/io/text/skipws.hpp>
#include <actl/io/text/text.hpp>
#include "test.hpp"

using namespace ac::io;

TEST_CASE("skipws") {
    std::string s = " \n\ta\n";
    memory<in> id{s};
    char a{};
    char b{};
    CHECK(read(id, skipws{} >>= text{}, a));
    CHECK(read(id, text{}, b));
    CHECK('a' == a);
    CHECK('\n' == b);
}
