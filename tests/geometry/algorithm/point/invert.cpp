// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/orientation/point_point.hpp>
#include <actl/geometry/algorithm/point/invert.hpp>
#include <actl/geometry/algorithm/point/norm.hpp>
#include "geometry/epsilon.hpp"
#include "test.hpp"

TEST_CASE("default") {
    point o{3, 4};
    circle<int> c{o, 2};
    for (int x : irange(9)) {
        for (int y : irange(9)) {
            point p{x, y};
            if (p != o) {
                auto i = invert(p, c);
                CHECK_NEAR(sqr(c.radius), norm(p - o) * norm(i - o), 1e-12);
                CHECK(collinear(absolute_error9, p - o, i - o));
                CHECK(0 < dot(p - o, i - o));
            }
        }
    }
}
