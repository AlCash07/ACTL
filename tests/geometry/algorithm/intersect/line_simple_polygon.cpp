// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/intersect/line_simple_polygon.hpp>
#include "test.hpp"

TEST_CASE("horizontal line all cases") {
    ray<int> seg{{0, 0}, {1, 0}};
    simple_polygon<point<int>> poly{{0, 3}, {0, -2}, {3, -2}, {1, 0}, {3, 2}, {5, 2},
                                    {7, 0}, {6, 0},  {5, -1}, {4, 0}, {5, 0}, {3, 1},
                                    {2, 0}, {3, 0},  {5, -2}, {9, 0}, {8, 0}, {5, 3}};
    std::vector<double> ts;
    intersect(line_scalar_policy{geometry_policy}, seg, poly, std::back_inserter(ts));
    sort(ts);
    CHECK_NEAR(std::vector{0, 1, 2, 5, 6, 9}, ts, 1e-12);
}
