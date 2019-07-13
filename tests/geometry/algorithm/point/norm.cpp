/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/point/norm.hpp>
#include <actl/test.hpp>

TEST("standard") { ASSERT_EQUAL(1.414213562373095, norm(point{1, 1}), 1e-12); }

TEST("comparable") {
    constexpr int x = 2'000'000'000;
    ASSERT_EQUAL(8'000'000'000'000'000'000LL,
                 sqr(norm(comparable_norm<long long>{}, point{x, -x})));
}
