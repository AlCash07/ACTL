// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/project/point_plane.hpp>
#include <actl/test.hpp>

TEST("default") {
    ASSERT_EQUAL(point{3, 0, -1}, project(point{2, -2, -4}, plane<int>{{1, 2, 3}, 0}), 1e-12);
}
