// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/geometry/algorithm/point/sort_by_angle.hpp>
#include <actl/range/irange.hpp>

std::vector<point<int>> get_points() {
    return {{2, 0}, {0, 2}, {-2, 0}, {0, -2}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1}, {0, 0}};
}

TEST_CASE("points") {
    auto points = get_points();
    sort_by_angle(points);
    std::vector<point<int>> expected{{0, 0},  {2, 0},   {1, 1},  {0, 2}, {-1, 1},
                                     {-2, 0}, {-1, -1}, {0, -2}, {1, -1}};
    CHECK(expected == points);
}

TEST_CASE("indices") {
    indexed_multi_point points{std::vector<int>{}, get_points()};
    auto r = irange((int)points.points.size());
    points.indices.assign(r.begin(), r.end());
    sort_by_angle(points, point{0, 0});
    CHECK(std::vector<int>{8, 0, 4, 1, 5, 2, 6, 3, 7} == points.indices);
}
