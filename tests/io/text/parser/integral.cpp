// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/text/parser/arithmetic.hpp>
#include "io/text/test_io.hpp"

using namespace ac::io;

TEST_CASE("read unsigned long long") {
    const auto max_ull = std::numeric_limits<unsigned long long>::max();
    text f;
    unsigned long long x{};
    test_read(0ull, "0", f);
    test_read(max_ull, "18446744073709551615", f);
    test_read<false>(x, "18446744073709551616", f);
    test_read<false>(x, "18446744073709551620", f);
    test_read<false>(x, "100000000000000000000", f);
    f.base = 2;
    test_read(0ull, "+00", f);
    test_read(max_ull, "1111111111111111111111111111111111111111111111111111111111111111", f);
    f.base = 3;
    test_read(max_ull, "11112220022122120101211020120210210211220", f);
    f.base = 36;
    test_read(max_ull, "3w5e11264sgsf", f);
    f.base = hex;
    test_read(max_ull, "fFfFfFfFfFfFfFfF", f);
    test_read(max_ull, "0XFFFFFFFFFFFFFFFF", f);
    test_read(max_ull, "0xffffffffffffffff", f);
    test_read<false>(x, "0xfffffffffffffffff", f);
    test_read(0ull, "0x0xf", f);
}

TEST_CASE("read long long") {
    const auto max_ull = std::numeric_limits<long long>::max();
    const auto min_ull = std::numeric_limits<long long>::min();
    text f;
    long long x{};
    test_read<true>(0ull, "+0", f);
    test_read<true>(min_ull, "-9223372036854775808", f);
    test_read<false>(x, "-9223372036854775809", f);
    test_read<true>(max_ull, "9223372036854775807", f);
    test_read<false>(x, "9223372036854775809", f);
    f.base = 2;
    test_read<true>(0ull, "0", f);
    test_read<true>(min_ull, "-1000000000000000000000000000000000000000000000000000000000000000",
                    f);
    test_read<true>(max_ull, "+111111111111111111111111111111111111111111111111111111111111111", f);
    f.base = 3;
    test_read<true>(min_ull, "-2021110011022210012102010021220101220222", f);
    test_read<true>(max_ull, "+2021110011022210012102010021220101220221", f);
    f.base = 36;
    test_read<true>(min_ull, "-1y2p0ij32e8e8", f);
    test_read<true>(max_ull, "+1y2p0ij32e8e7", f);
    f.base = hex;
    test_read<true>(min_ull, "-8000000000000000", f);
    test_read<true>(min_ull, "-0x8000000000000000", f);
    test_read<true>(max_ull, "7fffffffffffffff", f);
    test_read<true>(max_ull, "+0X7fFfFfFfFfFfFfFf", f);
    test_read<false>(x, "-8000000000000001", f);
    test_read<false>(x, "0x8000000000000000", f);
}

TEST_CASE("determin_ulle base") {
    text f{0, detect_base};
    test_read<true>(10, "10", f);
    test_read<true>(2, "0b10", f);
    test_read<true>(-2, "-0B10", f);
    test_read<true>(8, "010", f);
    test_read<true>(-16, "-0x10", f);
    test_read<true>(16, "0X10", f);
}

TEST_CASE("multiple signs") {
    text f;
    int x{};
    test_read<false>(x, "++1", f);
    test_read<false>(x, "+-1", f);
    test_read<false>(x, "--1", f);
    test_read<false>(x, "-+1", f);
}
