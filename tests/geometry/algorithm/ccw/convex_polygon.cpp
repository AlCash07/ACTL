/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/ccw/convex_polygon.hpp>
#include <actl/test.hpp>

TEST("default") {
    convex_polygon<point<int>> poly{{1, 0}, {1, 1}, {0, 1}};
    ASSERT_EQUAL(-1, ccw(poly));
}
