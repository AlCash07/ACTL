// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/geometry/algorithm/point/angle.hpp>
#include <actl/numeric/constants.hpp>

TEST_CASE("polar angle") {
    CHECK_NEAR(0, angle(point{1, 0}), 1e-12);
    CHECK_NEAR(math::pi / 4, angle(point{1, 1}), 1e-12);
    CHECK_NEAR(math::pi / 2, angle(point{0, 1}), 1e-12);
    CHECK_NEAR(math::pi * 3 / 4, angle(point{-1, 1}), 1e-12);
    CHECK_NEAR(math::pi, angle(point{-1, 0}), 1e-12);
    CHECK_NEAR(-math::pi * 3 / 4, angle(point{-1, -1}), 1e-12);
    CHECK_NEAR(-math::pi / 2, angle(point{0, -1}), 1e-12);
    CHECK_NEAR(-math::pi / 4, angle(point{1, -1}), 1e-12);
}

TEST_CASE("rotation angle") {
    point a{2, 2}, b{0, 2}, o{1, 1};
    CHECK_NEAR(math::pi / 2, angle(a, b, o), 1e-12);
    CHECK_NEAR(-math::pi / 2, angle(b, a, o), 1e-12);
}
