// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <catch2/catch_test_macros.hpp>

namespace ac {

template<typename T>
void test_relational_operators_for_equal_values(T const& l, T const& r) {
    REQUIRE(l == r);
    CHECK_FALSE(l != r);
    CHECK_FALSE(l < r);
    CHECK(l <= r);
    CHECK_FALSE(l > r);
    CHECK(l >= r);
}

template<typename T>
void test_relational_operators_for_different_values(
    T const& smaller, T const& bigger
) {
    REQUIRE(smaller < bigger);
    CHECK_FALSE(smaller == bigger);
    CHECK(smaller != bigger);
    CHECK(smaller <= bigger);
    CHECK_FALSE(smaller > bigger);
    CHECK_FALSE(smaller >= bigger);
}

template<typename T>
void test_relational_operators_noexcept(T const& t) {
    static_assert(noexcept(t == t));
    static_assert(noexcept(t != t));
    static_assert(noexcept(t < t));
    static_assert(noexcept(t <= t));
    static_assert(noexcept(t > t));
    static_assert(noexcept(t >= t));
}

} // namespace ac
