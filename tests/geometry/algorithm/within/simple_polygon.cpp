// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/within/simple_polygon.hpp>
#include "geometry/polygons.hpp"
#include "test.hpp"

TEST_CASE("star") {
    auto poly = get_star_polygon();
    for (auto p : poly) {
        CHECK(within::border == within(p, poly));
    }
    for (auto p : std::vector<point<int>>{{4, 2}, {-3, 2}}) {
        CHECK(within::border == within(p, poly));
    }
    for (auto p : std::vector<point<int>>{{0, 0}, {0, 4}, {-3, 1}, {-2, -3}, {2, -3}, {3, 1}}) {
        CHECK(within::inside == within(p, poly));
    }
    for (auto p : std::vector<point<int>>{{4, 1}, {1, 3}, {-4, 1}, {-3, -3}, {1, -3}, {0, -9}}) {
        CHECK(within::outside == within(p, poly));
    }
}
