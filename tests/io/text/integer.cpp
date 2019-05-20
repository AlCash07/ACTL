/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/text/integer.hpp>
#include <actl/io/text/manip.hpp>
#include <actl/test.hpp>
#include "test_output.hpp"

#include <actl/io/device/memory.hpp>

#include <cmath>
#include <iostream>

using namespace ac::io;

TEST("digit_count") {
    using io::detail::digit_count;
    ASSERT_EQUAL(2, digit_count(8, 3));
    ASSERT_EQUAL(9, digit_count(999'999'999, 10));
    ASSERT_EQUAL(10, digit_count(1'000'000'000, 10));
    ASSERT_EQUAL(64, digit_count(std::numeric_limits<unsigned long long>::max(), 2ull));
}

TEST("write unsigned long long") {
    const unsigned long long zero = 0;
    const auto max = std::numeric_limits<unsigned long long>::max();
    text_format<out> f;
    test_output("0", f, showpos, showbase, zero);
    test_output("18446744073709551615", f, max);
    test_output("0", f, setbase{2}, zero);
    test_output("1111111111111111111111111111111111111111111111111111111111111111", f, max);
    test_output("0", f, setbase{8}, zero);
    test_output("01777777777777777777777", f, max);
    test_output("1777777777777777777777", f, noshowbase, max);
    test_output("ffffffffffffffff", f, setbase{16}, max);
    test_output("FFFFFFFFFFFFFFFF", f, uppercase, max);
    test_output("0XFFFFFFFFFFFFFFFF", f, showbase, max);
    test_output("0xffffffffffffffff", f, nouppercase, max);
    test_output("0", f, zero);
    test_output("11112220022122120101211020120210210211220", f, setbase{3}, max);
    test_output("3w5e11264sgsf", f, setbase{36}, max);
}

TEST("write long long") {
    const long long zero = 0;
    const auto min = std::numeric_limits<long long>::min();
    const auto max = std::numeric_limits<long long>::max();
    text_format<out> f;
    test_output("0", f, showbase, zero);
    test_output("-9223372036854775808", f, min);
    test_output("9223372036854775807", f, max);
    test_output("+0", f, showpos, setbase{2}, zero);
    test_output("-1000000000000000000000000000000000000000000000000000000000000000", f, min);
    test_output("+111111111111111111111111111111111111111111111111111111111111111", f, max);
    test_output("0", f, noshowpos, setbase{8}, zero);
    test_output("-01000000000000000000000", f, min);
    test_output("0777777777777777777777", f, max);
    test_output("-1000000000000000000000", f, noshowbase, min);
    test_output("777777777777777777777", f, max);
    test_output("-8000000000000000", f, setbase{16}, min);
    test_output("7fffffffffffffff", f, max);
    test_output("7FFFFFFFFFFFFFFF", f, uppercase, max);
    test_output("-0X8000000000000000", f, showbase, min);
    test_output("0X7FFFFFFFFFFFFFFF", f, max);
    test_output("-0x8000000000000000", f, nouppercase, min);
    test_output("0x7fffffffffffffff", f, max);
    test_output("+0x7fffffffffffffff", f, showpos, max);
    test_output("+0", f, zero);
    test_output("-2021110011022210012102010021220101220222", f, setbase{3}, min);
    test_output("+2021110011022210012102010021220101220221", f, max);
    test_output("-1y2p0ij32e8e8", f, setbase{36}, min);
    test_output("+1y2p0ij32e8e7", f, max);
}
