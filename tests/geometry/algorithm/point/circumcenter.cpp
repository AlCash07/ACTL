// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/geometry/algorithm/point/circumcenter.hpp>
#include <actl/test.hpp>

TEST("default") {
    ASSERT_EQUAL(point{3, 1}, circumcenter(point{0, 0}, point{4, 4}, point{6, 0}), 1e-12);
}
