// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/plane.hpp>
#include <actl/test.hpp>

int rand1() { return ac::random.uniform(-800, 800); }

point<int> rand2() { return {rand1(), rand1()}; }

point<int, 3> rand3() { return {rand1(), rand1(), rand1()}; }

TEST("2d", repeat = 1000) {
    point a = rand2();
    point b = rand2();
    auto p = make_plane2d(a, b);
    ASSERT_EQUAL(0, p(a));
    ASSERT_EQUAL(0, p(b));
}

TEST("3d", repeat = 1000) {
    point a = rand3();
    point b = rand3();
    point c = rand3();
    auto p = make_plane3d(a, b, c);
    ASSERT_EQUAL(0, p(a));
    ASSERT_EQUAL(0, p(b));
    ASSERT_EQUAL(0, p(c));
}
