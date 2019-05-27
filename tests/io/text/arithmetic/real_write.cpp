/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/text/arithmetic/real_write.hpp>
#include <actl/io/text/manip.hpp>
#include <actl/test.hpp>
#include "../test_io.hpp"

using namespace ac::io;

TEST("write double") {
    const auto inf = std::numeric_limits<double>::infinity();
    const auto nan = std::numeric_limits<double>::quiet_NaN();
    text_format<out> f;
    test_write("inf", f, inf);
    test_write("-inf", f, -inf);
    test_write("+INF", f, showpos, uppercase, inf);
    test_write("NAN", f, noshowpos, nan);
    test_write("-NAN", f, -nan);
    test_write("+nan", f, showpos, nouppercase, nan);
    test_write("+0", f, setprecision{0}, 0.0);
    test_write("-0", f, -0.0);
    test_write("0", f, noshowpos, 0.0);
    test_write("0.", f, showpoint, 0.0);
    test_write("0.123456789", f, setprecision{9}, 0.123456789);
    test_write("-12345678987654320.000000000", f, -12345678987654320.0);
    test_write("F0E1.D20000000", f, uppercase, hex, 0xF0E1.D2p0);
}
