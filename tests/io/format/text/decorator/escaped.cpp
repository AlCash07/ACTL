// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/format/text/arithmetic/integer_write.hpp>
#include <actl/io/format/text/decorator/escaped.hpp>
#include <actl/io/format/text/text.hpp>
#include <actl/std/all.hpp>
#include "io/format/text/test_io.hpp"

using namespace ac::io;

auto ets() {
    return escaped{} >>= text_static<flag::boolalpha>{};
}

TEST_CASE("escaped int") {
    test_write("123", ets(), 123);
}

TEST_CASE("escaped char") {
    test_write("'0''\\n'", ets(), '0', '\n');
}

TEST_CASE("escaped string") {
    test_write(
        "\"\\0\\a\\b\\t\\n\\v\\f\\r\\\"\\'\\\\0rz\"",
        ets(),
        "\0\a\b\t\n\v\f\r\"\'\\0rz"sv
    );
}
