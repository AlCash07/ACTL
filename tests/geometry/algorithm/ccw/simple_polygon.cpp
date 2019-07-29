/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/ccw/simple_polygon.hpp>
#include <actl/test.hpp>

TEST("star") {
    simple_polygon<point<int>> poly{{0, -2}, {3, -4}, {2, 0},  {5, 2},  {1, 2},
                                    {0, 5},  {-1, 2}, {-5, 2}, {-2, 0}, {-3, -4}};
    ASSERT_EQUAL(-1, ccw(poly));
}
