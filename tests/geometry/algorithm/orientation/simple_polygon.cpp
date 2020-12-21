// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "geometry/polygons.hpp"
#include "test.hpp"
#include <actl/geometry/algorithm/orientation/simple_polygon.hpp>

TEST_CASE("star") {
    CHECK(orientation2d::left == orientation(get_star_polygon()));
}
