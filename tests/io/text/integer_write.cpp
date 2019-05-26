/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/text/integer_write.hpp>
#include <actl/io/text/manip.hpp>
#include "test_io.hpp"

using namespace ac::io;

TEST("write unsigned long long") {
    const unsigned long long zero = 0;
    const auto max = std::numeric_limits<unsigned long long>::max();
    text_format<out> f;
    test_write("0", f, showpos, showbase, zero);
    test_write("18446744073709551615", f, max);
    test_write("0", f, setbase{2}, zero);
    test_write("1111111111111111111111111111111111111111111111111111111111111111", f, max);
    test_write("0", f, setbase{8}, zero);
    test_write("01777777777777777777777", f, max);
    test_write("1777777777777777777777", f, noshowbase, max);
    test_write("ffffffffffffffff", f, setbase{16}, max);
    test_write("FFFFFFFFFFFFFFFF", f, uppercase, max);
    test_write("0XFFFFFFFFFFFFFFFF", f, showbase, max);
    test_write("0xffffffffffffffff", f, nouppercase, max);
    test_write("0", f, zero);
    test_write("11112220022122120101211020120210210211220", f, setbase{3}, max);
    test_write("3w5e11264sgsf", f, setbase{36}, max);
}

TEST("write long long") {
    const long long zero = 0;
    const auto min = std::numeric_limits<long long>::min();
    const auto max = std::numeric_limits<long long>::max();
    text_format<out> f;
    test_write("0", f, showbase, zero);
    test_write("-9223372036854775808", f, min);
    test_write("9223372036854775807", f, max);
    test_write("+0", f, showpos, setbase{2}, zero);
    test_write("-1000000000000000000000000000000000000000000000000000000000000000", f, min);
    test_write("+111111111111111111111111111111111111111111111111111111111111111", f, max);
    test_write("0", f, noshowpos, setbase{8}, zero);
    test_write("-01000000000000000000000", f, min);
    test_write("0777777777777777777777", f, max);
    test_write("-1000000000000000000000", f, noshowbase, min);
    test_write("777777777777777777777", f, max);
    test_write("-8000000000000000", f, setbase{16}, min);
    test_write("7fffffffffffffff", f, max);
    test_write("7FFFFFFFFFFFFFFF", f, uppercase, max);
    test_write("-0X8000000000000000", f, showbase, min);
    test_write("0X7FFFFFFFFFFFFFFF", f, max);
    test_write("-0x8000000000000000", f, nouppercase, min);
    test_write("0x7fffffffffffffff", f, max);
    test_write("+0x7fffffffffffffff", f, showpos, max);
    test_write("+0", f, zero);
    test_write("-2021110011022210012102010021220101220222", f, setbase{3}, min);
    test_write("+2021110011022210012102010021220101220221", f, max);
    test_write("-1y2p0ij32e8e8", f, setbase{36}, min);
    test_write("+1y2p0ij32e8e7", f, max);
}
