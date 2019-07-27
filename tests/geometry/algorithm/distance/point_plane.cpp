/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/distance/point_plane.hpp>
#include <actl/test.hpp>

TEST("default") {
    plane<int, 2> pl{{-1, 2}, {1, 1}};
    ASSERT_EQUAL(adl::sqrt(5.0), distance(pl, point{2, 4}), 1e-12);
    ASSERT_EQUAL(adl::sqrt(5.0), distance(pl, point{4, 0}), 1e-12);
}
