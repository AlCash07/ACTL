// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/geometry/point.hpp>

TEST_CASE("arithmetic") {
    point pi{1, 2};
    point pf{3.5f, 4.5f};
    point pd{5.5, 6.5};
    CHECK(point{-1, -2} == -pi);
    CHECK_ALMOST_EQUAL(point{4.5f, 6.5f}, pi + pf, 1e-9f);
    CHECK_ALMOST_EQUAL(point{4.5, 4.5}, pd - pi, 1e-12);
    CHECK(point{3, 6} == pi * 3);
    CHECK(point{4, 8} == 4 * pi);
    CHECK_ALMOST_EQUAL(point{2.75, 3.25}, pd / 2, 1e-12);
    CHECK(18.5, dot(pi, pd) == 1e-12);
}

TEST_CASE("inplace arithmetic") {
    point p = {1, 2};
    CHECK(point{3, 5} == p += {2, 3});
    CHECK(point{2, 6} == p -= {1, -1});
    CHECK(point{1, 3} == p /= 2);
    CHECK(point{3, 9} == p *= 3);
    CHECK(point{2, 3} == p = point<int>{point{2.5, 3.5}});
}

TEST_CASE("comparison") {
    CHECK(point{1, 2} == point{1, 2});
    CHECK(point{1, 2} > point{0, 3});
    CHECK(point{1, 2} <= point{1, 3});
}

TEST_CASE("2d perpendicular") {
    CHECK(point{-3, 2} == perpendicular(point{2, 3}));
}

TEST_CASE("3d cross product") {
    constexpr int M = 1'000'000'000;
    CHECK(point{-2, 999999999000000001LL, -1000000001000000001LL} ==
          cross(cast_before<Mul, long long>{}, point{1, M + 1, M - 1}, point{M + 1, M, M - 2}));
}
