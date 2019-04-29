/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/test.hpp>
#include <actl/util/span.hpp>

constexpr int N = 9;

TEST("array") {
    int a[N] = {};
    span s(a);
    ASSERT_EQUAL(a, s.data());
    ASSERT_EQUAL(N, s.size());
}

TEST("const vector") {
    const std::vector<double> v(N);
    span s(v);
    ASSERT_EQUAL(v.data(), s.data());
    ASSERT_EQUAL(N, s.size());
}
