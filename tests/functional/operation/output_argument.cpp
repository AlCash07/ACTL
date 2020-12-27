// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/functional/scalar/all.hpp>

TEST_CASE("inplace parameter") {
    int x = 4;
    CHECK(5 == ac::max(5, ac::inplace(x)));
    CHECK(5 == x);
    CHECK(5 == ac::max(ac::inplace(x), 3));
    CHECK(5 == x);
    CHECK(2 == (ac::inplace(x) -= 3));
    CHECK(2 == x);
}

TEST_CASE("output parameter") {
    int res{};
    CHECK(6 == ac::add(ac::out(res), 2, 4));
    CHECK(6 == res);
    ac::out(res) = ac::add(2, res);
    CHECK(8 == res);
}
