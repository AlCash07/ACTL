// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/operation/scalar/all.hpp>
#include "test.hpp"

TEST_CASE("expression assignment to out") {
    int res{};
    CHECK(6 == (ac::out{res} = ac::add(2, 4)));
    CHECK(6 == res);
    ac::out{res} = ac::add(2, res);
    CHECK(8 == res);
}
