/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/text/arithmetic/integer_read.hpp>
#include <actl/io/text/manip.hpp>
#include "test_io.hpp"

using namespace ac::io;

TEST("read unsigned long long") {
    const unsigned long long zero = 0;
    const auto max = std::numeric_limits<unsigned long long>::max();
    text_format<in> f;
    unsigned long long x{};
    test_read(zero, "0", f);
    test_read(max, "18446744073709551615", f);
    test_read<false>(x, "18446744073709551616", f);
    test_read<false>(x, "18446744073709551620", f);
    test_read<false>(x, "100000000000000000000", f);
    test_read(zero, "+00", f, setbase{2});
    test_read(max, "1111111111111111111111111111111111111111111111111111111111111111", f);
    test_read(max, "11112220022122120101211020120210210211220", f, setbase{3});
    test_read(max, "3w5e11264sgsf", f, setbase{36});
    test_read(max, "fFfFfFfFfFfFfFfF", f, setbase{16});
    test_read(max, "0XFFFFFFFFFFFFFFFF", f);
    test_read(max, "0xffffffffffffffff", f);
    test_read<false>(x, "0xfffffffffffffffff", f);
    test_read(zero, "0x0xf", f);
}

TEST("read long long") {
    const long long zero = 0;
    const auto max = std::numeric_limits<long long>::max();
    const auto min = std::numeric_limits<long long>::min();
    text_format<in> f;
    long long x{};
    test_read<true>(zero, "+0", f);
    test_read<true>(min, "-9223372036854775808", f);
    test_read<false>(x, "-9223372036854775809", f);
    test_read<true>(max, "9223372036854775807", f);
    test_read<false>(x, "9223372036854775809", f);
    test_read<true>(zero, "0", f, setbase{2});
    test_read<true>(min, "-1000000000000000000000000000000000000000000000000000000000000000", f);
    test_read<true>(max, "+111111111111111111111111111111111111111111111111111111111111111", f);
    test_read<true>(min, "-2021110011022210012102010021220101220222", f, setbase{3});
    test_read<true>(max, "+2021110011022210012102010021220101220221", f);
    test_read<true>(min, "-1y2p0ij32e8e8", f, setbase{36});
    test_read<true>(max, "+1y2p0ij32e8e7", f);
    test_read<true>(min, "-8000000000000000", f, setbase{16});
    test_read<true>(min, "-0x8000000000000000", f);
    test_read<true>(max, "7fffffffffffffff", f);
    test_read<true>(max, "+0X7fFfFfFfFfFfFfFf", f);
    test_read<false>(x, "-8000000000000001", f);
    test_read<false>(x, "0x8000000000000000", f);
}

TEST("determine base") {
    text_format<in> f;
    test_read<true>(10, "10", f, setbase{0});
    test_read<true>(8, "010", f);
    test_read<true>(-16, "-0x10", f);
    test_read<true>(16, "0X10", f);
}

TEST("multiple signs") {
    text_format<in> f;
    int x{};
    test_read<false>(x, "++1", f);
    test_read<false>(x, "+-1", f);
    test_read<false>(x, "--1", f);
    test_read<false>(x, "-+1", f);
}
