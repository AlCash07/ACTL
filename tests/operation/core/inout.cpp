// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/operation/scalar/all.hpp>
#include "test.hpp"

TEST_CASE("inout parameter") {
    int x = 4;
    CHECK(5 == ac::max(5, ac::inout(x)));
    CHECK(5 == x);
    CHECK(5 == ac::max(ac::inout(x), 3));
    CHECK(5 == x);
    CHECK(2 == (ac::inout(x) -= 3));
    CHECK(2 == x);
}
