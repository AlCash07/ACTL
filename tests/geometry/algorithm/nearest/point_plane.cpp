// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/geometry/algorithm/nearest/point_plane.hpp>

TEST_CASE("default") {
    CHECK_ALMOST_EQUAL(point{-1, -1, 1}, nearest(point{0, 1, 4}, plane<int>{{1, 2, 3}, 0}).second,
                       1e-12);
}
