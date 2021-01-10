// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/io/device/memory.hpp>
#include <actl/io/text/text.hpp>
#include <actl/io/utility/skip.hpp>
#include "test.hpp"

using namespace ac::io;

TEST_CASE("whitespace skip") {
    memory<in> id{" \n\tb"};
    char a, b;
    CHECK(read(id, text{}, a, ws, b));
    CHECK(' ' == a);
    CHECK('b' == b);
}
