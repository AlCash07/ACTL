/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/point/collinear.hpp>
#include <actl/geometry/algorithm/point/invert.hpp>
#include <actl/geometry/algorithm/point/norm.hpp>
#include <actl/test.hpp>
#include "geometry/epsilon.hpp"

TEST("default") {
    point o{3, 4};
    circle<int> c{o, 2};
    for (int x : irange(9)) {
        for (int y : irange(9)) {
            point p{x, y};
            if (p != o) {
                auto i = invert(p, c);
                ASSERT_EQUAL(sqr(c.radius), norm(p - o) * norm(i - o), 1e-12);
                ASSERT_TRUE(collinear(absolute_error9, p - o, i - o));
                ASSERT_TRUE(0 < dot(p - o, i - o));
            }
        }
    }
}
