// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/numeric/algorithm/binary_pow.hpp>
#include "test.hpp"

TEST_CASE("binary_pow correctness") {
    CHECK(1 == binary_pow(1024, 0));
    CHECK(1 << 15 == binary_pow(2, 15));
    CHECK(1 << 16 == binary_pow(2, 16));
    CHECK(1ll << 62 == binary_pow(2ll, 62));
}
