// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/rotate/point2d.hpp>
#include <actl/numeric/constants.hpp>
#include <actl/test.hpp>

TEST("default") {
    const double sqrt2 = math::sqrt(2.0);
    ASSERT_EQUAL(point{sqrt2, sqrt2}, rotate(point{2, 0}, math::pi / 4), 1e-12);
    ASSERT_EQUAL(point{-2, 0}, rotate(point{2, 0}, math::pi), 1e-12);
    ASSERT_EQUAL(point{sqrt2, -sqrt2}, rotate(point{2, 0}, math::pi * 7 / 4), 1e-12);
}
