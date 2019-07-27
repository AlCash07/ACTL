/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/3d/point3d.hpp>
#include <actl/test.hpp>

TEST("cross") {
    constexpr int M = 1'000'000'000;
    ASSERT_EQUAL(
        point{-2, 999999999000000001LL, -1000000001000000001LL},
        cross(op::product_policy<long long>{}, point{1, M + 1, M - 1}, point{M + 1, M, M - 2}));
}
