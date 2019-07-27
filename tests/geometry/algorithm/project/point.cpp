/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/project/point.hpp>
#include <actl/test.hpp>

TEST("default") { ASSERT_EQUAL(point{2, 4}, project(point{9, 1}, point{2, 4})); }
