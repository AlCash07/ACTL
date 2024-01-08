/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at
 *http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/operations.hpp>
#include <actl/test.hpp>

TEST("2d perpendicular")
{
    ASSERT_EQUAL(vec{-3, 2}, perpendicular(vec{2, 3}));
}

TEST("3d cross product")
{
    constexpr int M = 1'000'000'000;
    ASSERT_EQUAL(
        vec{-2, 999999999000000001LL, -1000000001000000001LL},
        cross(
            math::cast_before<math::Mul, long long>{},
            vec{1, M + 1, M - 1},
            vec{M + 1, M, M - 2}));
}
