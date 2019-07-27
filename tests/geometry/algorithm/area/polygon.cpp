/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/area/polygon.hpp>
#include <actl/test.hpp>

TEST("trident") {
    polygon<point<int>> poly{{-2, 0}, {-2, 4}, {-1, 1}, {0, 4},  {1, 1},
                             {2, 4},  {2, 0},  {1, 0},  {0, -1}, {-1, 0}};
    ASSERT_EQUAL(-22, area(poly));
}
