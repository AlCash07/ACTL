/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/device/memory.hpp>
#include <actl/io/text/manip.hpp>
#include <actl/io/text/text.hpp>
#include <actl/io/util/formatted.hpp>
#include <actl/test.hpp>

using namespace ac::io;

TEST("whitespace skip") {
    std::string s = " \n\ta\n";
    formatted<memory<in>> id{s};
    char a, b;
    ASSERT_TRUE(read(id, skipws, a, noskipws, b));
    ASSERT_EQUAL('a', a);
    ASSERT_EQUAL('\n', b);
}
