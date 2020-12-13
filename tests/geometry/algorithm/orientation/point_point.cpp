// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/orientation/point_point.hpp>
#include <actl/test.hpp>

TEST("2d default") {
    point a{6, 2}, b{8, 7}, c{1, 9};
    ASSERT_EQUAL(orientation2d::left, orientation(a, b, c));
    ASSERT_EQUAL(orientation2d::left, orientation(b, c, a));
    ASSERT_EQUAL(orientation2d::left, orientation(c, a, b));
    ASSERT_EQUAL(orientation2d::right, orientation(a, c, b));
    ASSERT_EQUAL(orientation2d::right, orientation(b, a, c));
    ASSERT_EQUAL(orientation2d::right, orientation(c, b, a));
}

TEST("2d collinear") {
    point a{2, 1};
    ASSERT_EQUAL(orientation2d::collinear, orientation(2 * a, 5 * a));
    ASSERT_EQUAL(orientation2d::collinear, orientation(-a, 3 * a));
}

TEST("3d") {
    std::vector<point<int, 3>> v;
    for (int x : irange(-1, 2)) {
        for (int y : irange(-1, 2)) {
            for (int z : irange(-1, 2)) {
                if (x != 0 || y != 0 || z != 0) v.emplace_back(x, y, z);
            }
        }
    }
    for (auto p0 : v) {
        for (auto p1 : v) {
            ASSERT_EQUAL(p0 == p1 || p0 == -p1, collinear(p0, p1));
        }
    }
}
