// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/operation/scalar/bit/all.hpp>
#include "test.hpp"

TEST_CASE("bit operations") {
    STATIC_ASSERT_WORKAROUND(-2 == ac::bit_not(1));
    STATIC_ASSERT_WORKAROUND(1 == ac::bit_and(3, 5));
    STATIC_ASSERT_WORKAROUND(7 == ac::bit_or(3, 5));
    STATIC_ASSERT_WORKAROUND(6 == ac::bit_xor(3, 5));
}
