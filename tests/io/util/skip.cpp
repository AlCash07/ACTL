/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/device/memory.hpp>
#include <actl/io/util/skip.hpp>
#include <actl/test.hpp>

using namespace ac::io;

TEST("whitespace skip") {
    memory<in> id{" \n\tb"};
    char a, b;
    ASSERT_TRUE(read(id, text{}, a, ws, b));
    ASSERT_EQUAL(' ', a);
    ASSERT_EQUAL('b', b);
}
