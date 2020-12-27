// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "io/text/test_io.hpp"
#include <actl/io/text/arithmetic/integer_write.hpp>

using namespace ac::io;

TEST_CASE("write unsigned long long") {
    const auto max_ull = std::numeric_limits<unsigned long long>::max();
    text f;
    test_write("0", f, showpos, showbase, 0ull);
    test_write("18446744073709551615", f, max_ull);
    f.base = 2;
    test_write("0", f, 0ull);
    test_write("1111111111111111111111111111111111111111111111111111111111111111", f, max_ull);
    f.base = octal;
    test_write("0", f, 0ull);
    test_write("01777777777777777777777", f, max_ull);
    test_write("1777777777777777777777", f, noshowbase, max_ull);
    f.base = hex;
    test_write("ffffffffffffffff", f, max_ull);
    test_write("FFFFFFFFFFFFFFFF", f, uppercase, max_ull);
    test_write("0XFFFFFFFFFFFFFFFF", f, showbase, max_ull);
    test_write("0xffffffffffffffff", f, nouppercase, max_ull);
    test_write("0", f, 0ull);
    f.base = 3;
    test_write("11112220022122120101211020120210210211220", f, max_ull);
    f.base = 36;
    test_write("3w5e11264sgsf", f, max_ull);
}

TEST_CASE("write long long") {
    const auto min_ll = std::numeric_limits<long long>::min();
    const auto max_ll = std::numeric_limits<long long>::max();
    text f;
    test_write("0", f, showbase, 0ll);
    test_write("-9223372036854775808", f, min_ll);
    test_write("9223372036854775807", f, max_ll);
    f.base = 2;
    test_write("+0", f, showpos, 0ll);
    test_write("-1000000000000000000000000000000000000000000000000000000000000000", f, min_ll);
    test_write("+111111111111111111111111111111111111111111111111111111111111111", f, max_ll);
    f.base = octal;
    test_write("0", f, noshowpos, 0ll);
    test_write("-01000000000000000000000", f, min_ll);
    test_write("0777777777777777777777", f, max_ll);
    test_write("-1000000000000000000000", f, noshowbase, min_ll);
    test_write("777777777777777777777", f, max_ll);
    f.base = hex;
    test_write("-8000000000000000", f, min_ll);
    test_write("7fffffffffffffff", f, max_ll);
    test_write("7FFFFFFFFFFFFFFF", f, uppercase, max_ll);
    test_write("-0X8000000000000000", f, showbase, min_ll);
    test_write("0X7FFFFFFFFFFFFFFF", f, max_ll);
    test_write("-0x8000000000000000", f, nouppercase, min_ll);
    test_write("0x7fffffffffffffff", f, max_ll);
    test_write("+0x7fffffffffffffff", f, showpos, max_ll);
    test_write("+0", f, 0ll);
    f.base = 3;
    test_write("-2021110011022210012102010021220101220222", f, min_ll);
    test_write("+2021110011022210012102010021220101220221", f, max_ll);
    f.base = 36;
    test_write("-1y2p0ij32e8e8", f, min_ll);
    test_write("+1y2p0ij32e8e7", f, max_ll);
}
