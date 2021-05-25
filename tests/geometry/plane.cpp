// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/plane.hpp>
#include "test.hpp"

int rand1()
{
    return ac::random.uniform(-800, 800);
}

point<int> rand2()
{
    return {rand1(), rand1()};
}

point<int, 3> rand3()
{
    return {rand1(), rand1(), rand1()};
}

TEST_CASE("2d", repeat = 1000)
{
    point a = rand2();
    point b = rand2();
    auto p = make_plane2d(a, b);
    CHECK(0 == p(a));
    CHECK(0 == p(b));
}

TEST_CASE("3d", repeat = 1000)
{
    point a = rand3();
    point b = rand3();
    point c = rand3();
    auto p = make_plane3d(a, b, c);
    CHECK(0 == p(a));
    CHECK(0 == p(b));
    CHECK(0 == p(c));
}
