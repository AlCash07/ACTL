/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/crosses/plane_sphere.hpp>
#include <actl/test.hpp>

TEST("all cases") {
    point v{3, 4, 12};
    sphere<int> s{{0, 0, 0}, 13};
    ASSERT_FALSE(crosses(plane{v, 168}, s));
    ASSERT_TRUE(crosses(plane{v, 169}, s));
    ASSERT_TRUE(crosses(plane{v, 170}, s));
}
