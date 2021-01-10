// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/polygon/max_distance.hpp>
#include "test.hpp"

TEST_CASE("default") {
    convex_polygon<point<double>> poly{{-0.9846, -1.53251}, {3.88558, -2.37188},
                                       {3.92734, -2.37856}, {4.02136, -1.57843},
                                       {0.79916, 0.98291},  {0.49946, 1.19525}};
    auto dist = max_distance(poly);
    CHECK_NEAR(dist, distance(poly[0], poly[3]), 1e-12);
}
