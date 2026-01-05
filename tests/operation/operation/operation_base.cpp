// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/numeric/arithmetic/additive/add.hpp>
#include "test.hpp"

TEST_CASE("operation_base with ac::inout argument is immediately evaluated") {
    int x = 2;
    int y = 3;
    /* Any compatible argument can be ac::inout. */
    /* Resulting reference is returned. */
    REQUIRE(5 == ac::add(ac::inout{x}, y));
    REQUIRE(5 == x);
    REQUIRE(3 == y); // other argument is unchanged
    REQUIRE(8 == ac::add(x, ac::inout{y}));
    REQUIRE(8 == y);
    REQUIRE(5 == x); // other argument is unchanged
    /* The same result can be achieved by using
       assignment operators on ac::inout. */
    REQUIRE(13 == (ac::inout{x} += y));
    REQUIRE(13 == x);
    REQUIRE(8 == y); // other argument is unchanged
}
