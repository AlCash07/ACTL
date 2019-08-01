/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/crosses/plane_plane.hpp>
#include <actl/test.hpp>

TEST("all cases") {
    ASSERT_TRUE(crosses(plane<int>{{4, 3, 2}, 5}, plane<int>{{4, 3, 1}, -2}));
    ASSERT_FALSE(crosses(plane<int>{{4, 3, -1}, 5}, plane<int>{{4, 3, -1}, -2}));
    ASSERT_TRUE(crosses(plane<int>{{4, 3, -1}, 5}, plane<int>{{4, 3, -1}, 5}));
}
