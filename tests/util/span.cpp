// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/util/span.hpp>

constexpr ac::index N = 9;

TEST_CASE("empty span") {
    span<int> s;
    CHECK_FALSE(s.data());
    span<int, 0> s0;
    CHECK_FALSE(s0.data());
}

TEST_CASE("span from array") {
    int a[N] = {};
    span s{a};
    CHECK(static_cast<int*>(a) == s.data());
    static_assert(N == s.size());
}

TEST_CASE("span from const vector") {
    const std::vector<double> v(N);
    span s{v};
    CHECK(v.data() == s.data());
    CHECK(N == s.size());
}
