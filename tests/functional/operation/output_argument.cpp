// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/functional/scalar/all.hpp>

TEST_CASE("inplace parameter") {
    int x = 4;
    CHECK(5 == math::max(5, inplace(x)));
    CHECK(5 == x);
    CHECK(5 == math::max(inplace(x), 3));
    CHECK(5 == x);
    CHECK(2 == (inplace(x) -= 3));
    CHECK(2 == x);
}

TEST_CASE("output parameter") {
    int res{};
    CHECK(6 == math::add(out(res), 2, 4));
    CHECK(6 == res);
    out(res) = math::add(2, res);
    CHECK(8 == res);
}
