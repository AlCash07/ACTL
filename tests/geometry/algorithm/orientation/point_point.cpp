// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/orientation/point_point.hpp>
#include "test.hpp"

TEST_CASE("2d default") {
    point a{6, 2}, b{8, 7}, c{1, 9};
    CHECK(orientation2d::left == orientation(a, b, c));
    CHECK(orientation2d::left == orientation(b, c, a));
    CHECK(orientation2d::left == orientation(c, a, b));
    CHECK(orientation2d::right == orientation(a, c, b));
    CHECK(orientation2d::right == orientation(b, a, c));
    CHECK(orientation2d::right == orientation(c, b, a));
}

TEST_CASE("2d collinear") {
    point a{2, 1};
    CHECK(orientation2d::collinear == orientation(2 * a, 5 * a));
    CHECK(orientation2d::collinear == orientation(-a, 3 * a));
}

TEST_CASE("3d") {
    std::vector<point<int, 3>> v;
    for (int x : irange(-1, 2)) {
        for (int y : irange(-1, 2)) {
            for (int z : irange(-1, 2)) {
                if (x != 0 || y != 0 || z != 0)
                    v.emplace_back(x, y, z);
            }
        }
    }
    for (auto p0 : v) {
        for (auto p1 : v) {
            CHECK(p0 == p1 || p0 == -p1 == collinear(p0, p1));
        }
    }
}
