// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "io/text/test_io.hpp"
#include <actl/io/device/memory.hpp>
#include <actl/io/text/skipws.hpp>
#include <actl/io/text/text.hpp>
#include <actl/range/traits.hpp>
#include <actl/std/string.hpp>

static_assert(ac::is_container_v<std::string>);
static_assert(ac::is_sequence_range_v<std::string>);
static_assert(ac::is_random_access_range_v<std::string>);
static_assert(ac::is_contiguous_range_v<std::string>);
static_assert(!ac::is_associative_range_v<std::string>);

using namespace ac::io;

TEST_CASE("string read") {
    std::string s = "  01234567890123456789 0123456789";
    std::string x;
    CHECK(read(memory<in>{s}, skipws{} >>= text{}, x));
    CHECK("01234567890123456789"sv == x);
}

TEST_CASE("string write") {
    test_write("abacaba", text{}, std::string{"abacaba"});
}
