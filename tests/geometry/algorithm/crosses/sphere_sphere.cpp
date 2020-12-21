// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/geometry/algorithm/crosses/sphere_sphere.hpp>

TEST_CASE("all cases") {
    math::defer_sqrt policy;
    circle<int> c{{0, 0}, 5};
    CHECK_FALSE(crosses(policy, c, circle<int>{{0, 2}, 2}));
    CHECK(crosses(policy, c, circle<int>{{0, 3}, 2}));
    CHECK(crosses(policy, c, circle<int>{{0, 5}, 2}));
    CHECK(crosses(policy, c, circle<int>{{0, 7}, 2}));
    CHECK_FALSE(crosses(policy, c, circle<int>{{0, 8}, 2}));
}
