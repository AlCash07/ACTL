// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/operation/scalar/arithmetic/all.hpp>
#include "test.hpp"

TEST_CASE("arithmetic") {
    STATIC_ASSERT_WORKAROUND(-6 == ac::neg(6));
    STATIC_ASSERT_WORKAROUND(2 == ac::neg(-2));
    STATIC_ASSERT_WORKAROUND(8 == ac::add(6, 2));
    STATIC_ASSERT_WORKAROUND(4 == ac::sub(6, 2));
    STATIC_ASSERT_WORKAROUND(12 == ac::mul(6, 2));
    STATIC_ASSERT_WORKAROUND(3 == ac::div(7, 2));
}
