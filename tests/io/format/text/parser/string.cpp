// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/device/memory.hpp>
#include <actl/io/format/text/parser/string.hpp>
#include <actl/io/format/text/skipws.hpp>
#include <actl/io/format/text/text.hpp>
#include "io/format/text/test_io.hpp"

using namespace ac::io;

TEST_CASE("string read")
{
    std::string s = "  01234567890123456789 0123456789";
    std::string x;
    CHECK(read(memory<in>{s}, skipws{} >>= text{}, x));
    CHECK("01234567890123456789"sv == x);
}

TEST_CASE("string write")
{
    test_write("abacaba", text{}, std::string{"abacaba"});
}
