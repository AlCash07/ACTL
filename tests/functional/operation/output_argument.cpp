// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/scalar/all.hpp>
#include <actl/test.hpp>

TEST("inplace parameter") {
    int x = 4;
    ASSERT_EQUAL(5, math::max(5, inplace(x)));
    ASSERT_EQUAL(5, x);
    ASSERT_EQUAL(5, math::max(inplace(x), 3));
    ASSERT_EQUAL(5, x);
    ASSERT_EQUAL(2, inplace(x) -= 3);
    ASSERT_EQUAL(2, x);
}

TEST("output parameter") {
    int res{};
    ASSERT_EQUAL(6, math::add(out(res), 2, 4));
    ASSERT_EQUAL(6, res);
    out(res) = math::add(2, res);
    ASSERT_EQUAL(8, res);
}
