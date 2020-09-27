/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/text/parser/arithmetic.hpp>
#include <actl/test.hpp>

#include "io/text/test_io.hpp"

using namespace ac::io;

TEST("read double") {
    text f;
    double x;
    test_read<false>(x, ".1", f);
    ASSERT_TRUE(read(io::string<io::in>{"+0.123456789"}, f, x));
    ASSERT_EQUAL(0.123456789, x, 1e-12);
    ASSERT_TRUE(read(io::string<io::in>{"-12345678987654320.000000000"}, f, x));
    ASSERT_EQUAL(-12345678987654320.0, x, 1e-12);
}
