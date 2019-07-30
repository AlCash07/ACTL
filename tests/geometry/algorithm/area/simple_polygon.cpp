/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/area/simple_polygon.hpp>
#include <actl/test.hpp>
#include "geometry/polygons.hpp"

TEST("trident") {
    auto poly = get_trident_polygon();
    reverse(poly);
    ASSERT_EQUAL(-22, area(poly));
}
