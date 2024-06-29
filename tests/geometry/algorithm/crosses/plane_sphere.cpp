// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/crosses/plane_sphere.hpp>
#include "test.hpp"

TEST_CASE("all cases") {
    point v{3, 4, 12};
    sphere<int> s{{0, 0, 0}, 13};
    CHECK_FALSE(crosses(plane{v, 168}, s));
    CHECK(crosses(plane{v, 169}, s));
    CHECK(crosses(plane{v, 170}, s));
}
