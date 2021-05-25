// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/text/arithmetic/real_write.hpp>
#include "io/text/test_io.hpp"
#include "test.hpp"

using namespace ac::io;

TEST_CASE("write double")
{
    const auto inf = std::numeric_limits<double>::infinity();
    const auto nan = std::numeric_limits<double>::quiet_NaN();
    text f;
    test_write("inf", f, inf);
    test_write("-inf", f, -inf);
    test_write("+INF", f, showpos, uppercase, inf);
    test_write("NAN", f, noshowpos, nan);
    test_write("-NAN", f, -nan);
    test_write("+nan", f, showpos, nouppercase, nan);
    f.precision = 0;
    test_write("+0", f, 0.0);
    test_write("-0", f, -0.0);
    test_write("0", f, noshowpos, 0.0);
    test_write("0.", f, showpoint, 0.0);
    f.precision = 9;
    test_write("0.123456789", f, 0.123456789);
    test_write("-12345678987654320.000000000", f, -12345678987654320.0);
    f.base = hex;
    test_write("F0E1.D20000000", f, uppercase, 0xF0E1.D2p0);
}
