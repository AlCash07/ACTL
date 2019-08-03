/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/crosses/sphere_sphere.hpp>
#include <actl/test.hpp>

TEST("all cases") {
    op::defer_sqrt policy;
    circle<int> c{{0, 0}, 5};
    ASSERT_FALSE(crosses(policy, c, circle<int>{{0, 2}, 2}));
    ASSERT_TRUE(crosses(policy, c, circle<int>{{0, 3}, 2}));
    ASSERT_TRUE(crosses(policy, c, circle<int>{{0, 5}, 2}));
    ASSERT_TRUE(crosses(policy, c, circle<int>{{0, 7}, 2}));
    ASSERT_FALSE(crosses(policy, c, circle<int>{{0, 8}, 2}));
}
