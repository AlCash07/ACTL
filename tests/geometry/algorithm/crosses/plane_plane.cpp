// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/geometry/algorithm/crosses/plane_plane.hpp>

TEST_CASE("all cases") {
    CHECK(crosses(plane<int>{{4, 3, 2}, 5}, plane<int>{{4, 3, 1}, -2}));
    CHECK_FALSE(crosses(plane<int>{{4, 3, -1}, 5}, plane<int>{{4, 3, -1}, -2}));
    CHECK(crosses(plane<int>{{4, 3, -1}, 5}, plane<int>{{4, 3, -1}, 5}));
}
