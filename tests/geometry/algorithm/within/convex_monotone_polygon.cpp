// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/within/convex_monotone_polygon.hpp>
#include "geometry/polygons.hpp"
#include "test.hpp"

TEST_CASE("octagon") {
    auto poly = get_octagon();
    for (auto p : poly) {
        CHECK(within::border == within(p, poly));
    }
    for (auto p : std::vector<point<int>>{
             {-2, -2},
             {2, -2},
             {2, 2},
             {-2, 2},
             {0, -3},
             {3, 0},
             {0, 3},
             {-3, 0}
         }) {
        CHECK(within::border == within(p, poly));
    }
    for (auto p : std::vector<point<int>>{
             {0, 0},
             {-2, -1},
             {-1, -2},
             {1, -2},
             {2, -1},
             {2, 1},
             {1, 2},
             {-1, 2},
             {-2, 1}
         }) {
        CHECK(within::inside == within(p, poly));
    }
    for (auto p : std::vector<point<int>>{
             {-6, -2}, {6, 2}, {0, -4}, {4, 0}, {0, 4}, {-4, 0}
         }) {
        CHECK(within::outside == within(p, poly));
    }
    for (auto p : std::vector<point<int>>{
             {-3, -2},
             {-2, -3},
             {2, -3},
             {3, -2},
             {3, 2},
             {2, 3},
             {-2, 3},
             {-3, 2}
         }) {
        CHECK(within::outside == within(p, poly));
    }
}
