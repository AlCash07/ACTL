// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/functional/scalar/arithmetic.hpp>

TEST_CASE("arithmetic") {
    static_assert(-6 == math::neg(6));
    static_assert(2 == math::neg(-2));
    static_assert(8 == math::add(6, 2));
    static_assert(4 == math::sub(6, 2));
    static_assert(12 == math::mul(6, 2));
    static_assert(3 == math::div(7, 2));
}
