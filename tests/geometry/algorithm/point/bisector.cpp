/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/orientation/point_point.hpp>
#include <actl/geometry/algorithm/point/bisector.hpp>
#include <actl/test.hpp>
#include "geometry/epsilon.hpp"

TEST("default") {
    ASSERT_TRUE(collinear(absolute_error9, point{1, -1}, bisector(point{6, -3}, point{2, -4})));
}
