// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "geometry/polygons.hpp"
#include "test.hpp"
#include <actl/geometry/algorithm/within/monotone_polygon.hpp>

TEST_CASE("trident") {
    auto poly = get_trident_polygon();
    for (auto& p : poly)
        p *= 2;
    for (auto p : poly) {
        CHECK(within::border == within(p, poly));
    }
    for (auto p : std::vector<point<int>>{
             {-3, 0}, {-1, -1}, {1, -1}, {3, 0}, {4, 3}, {3, 5}, {1, 5}, {-1, 5}, {-3, 5}}) {
        CHECK(within::border == within(p, poly));
    }
    for (auto p : std::vector<point<int>>{
             {-3, 1}, {-2, 1}, {-1, 0}, {0, -1}, {0, 7}, {1, 4}, {2, 1}, {3, 4}}) {
        CHECK(within::inside == within(p, poly));
    }
    for (auto p : std::vector<point<int>>{{-5, 3}, {-3, -1}, {-3, 6}, {-1, -2}, {-1, 6}, {0, -3}}) {
        CHECK(within::outside == within(p, poly));
    }
    for (auto p : std::vector<point<int>>{{0, 9}, {2, -1}, {2, 3}, {4, -1}, {4, 9}, {5, -1}}) {
        CHECK(within::outside == within(p, poly));
    }
}
