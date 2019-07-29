/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/project/line.hpp>
#include <actl/test.hpp>

TEST("scalar") {
    ASSERT_EQUAL(
        0.5, project(line_scalar_policy{geometry_policy}, point{2, 4}, line<int>{{1, 1}, {5, 3}}),
        1e-12);
}

TEST("point") { ASSERT_EQUAL(point{3, 2}, project(point{2, 4}, line<int>{{1, 1}, {5, 3}}), 1e-12); }
