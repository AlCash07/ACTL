/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/geometry/line.hpp>
#include <actl/test.hpp>

TEST("any_line") {
    point a{3, 1};
    point b{1, 5};
    auto l = make_any_line(a, endpoint::open, b, endpoint::free);
    ASSERT_EQUAL(a, l.begin);
    ASSERT_EQUAL(b, l.end());
    ASSERT_EQUAL(point{2, 3}, l(0.5), 1e-12);
}
