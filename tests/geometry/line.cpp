// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/line.hpp>
#include "test.hpp"

TEST_CASE("any_line")
{
    point a{3, 1};
    point b{1, 5};
    auto l = make_any_line(a, endpoint::open, b, endpoint::free);
    CHECK(a == l.begin);
    CHECK(b == l.end());
    CHECK_NEAR(point{2, 3}, l(0.5), 1e-12);
}
