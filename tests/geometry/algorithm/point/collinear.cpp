/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/algorithm/point/collinear.hpp>
#include <actl/test.hpp>

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
