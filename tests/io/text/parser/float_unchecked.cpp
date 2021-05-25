// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/io/text/parser/arithmetic.hpp>
#include "io/text/test_io.hpp"

using namespace ac::io;

TEST_CASE("read double")
{
    text f;
    double x;
    test_read<false>(x, ".1", f);
    CHECK(read(io::string<io::in>{"+0.123456789"}, f, x));
    CHECK_NEAR(0.123456789, x, 1e-12);
    CHECK(read(io::string<io::in>{"-12345678987654320.000000000"}, f, x));
    CHECK_NEAR(-12345678987654320.0, x, 1e-12);
}
