// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/area/point_line.hpp>
#include "test.hpp"

TEST_CASE("default") {
    CHECK(4 == area(line<int>{{1, 1}, {1, 3}}, point{-1, 2222}));
}
