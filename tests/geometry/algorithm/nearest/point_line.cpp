// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/geometry/algorithm/nearest/point_line.hpp>

TEST_CASE("vertical segment") {
    segment<int> l{{1, 1}, {1, 5}};
    CHECK_ALMOST_EQUAL(point{1, 1}, nearest(point{4, -1}, l).second, 1e-12);
    CHECK_ALMOST_EQUAL(point{1, 3}, nearest(point{-2, 3}, l).second, 1e-12);
    CHECK_ALMOST_EQUAL(point{1, 5}, nearest(point{1, 7}, l).second, 1e-12);
}

TEST_CASE("vertical line") {
    line<int> l{{1, 1}, {1, 5}};
    CHECK_ALMOST_EQUAL(point{1, -1}, nearest(point{4, -1}, l).second, 1e-12);
    CHECK_ALMOST_EQUAL(point{1, 3}, nearest(point{-2, 3}, l).second, 1e-12);
    CHECK_ALMOST_EQUAL(point{1, 7}, nearest(point{1, 7}, l).second, 1e-12);
}
