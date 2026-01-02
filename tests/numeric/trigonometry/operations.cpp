// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/numeric/comparison/all.hpp>
#include <actl/numeric/trigonometry/operations.hpp>
#include "test.hpp"

TEST_CASE("trigonometric operations") {
    constexpr auto epsilon = 1e-9;
    CHECK(ac::abs(ac::sin(0)) < epsilon);
    CHECK(ac::abs(ac::cos(0) - 1) < epsilon);
    CHECK(ac::abs(ac::atan2(0, 1)) < epsilon);
}
