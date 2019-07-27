/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/distance/distance.hpp>
#include <actl/geometry/algorithm/distance/point_point.hpp>
#include <actl/test.hpp>

TEST("default") { ASSERT_EQUAL(5.0, distance(point{2, 2}, point{5, 6}), 1e-12); }

TEST("deferred") { ASSERT_EQUAL(2, sqr(distance(op::defer_sqrt{}, point{2, 2}, point{3, 1}))); }
