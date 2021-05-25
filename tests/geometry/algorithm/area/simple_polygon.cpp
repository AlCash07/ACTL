// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/area/simple_polygon.hpp>
#include "geometry/polygons.hpp"
#include "test.hpp"

TEST_CASE("trident")
{
    auto poly = get_trident_polygon();
    reverse(poly);
    CHECK(-22 == area(poly));
}
