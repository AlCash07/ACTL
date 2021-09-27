#pragma once

#include <catch2/catch_test_macros.hpp>

namespace ac {

template <class T>
void test_relational_operators_for_equal_values(const T& x, const T& y)
{
    REQUIRE(x == y);
    CHECK_FALSE(x != y);
    CHECK_FALSE(x < y);
    CHECK(x <= y);
    CHECK_FALSE(x > y);
    CHECK(x >= y);
}

template <class T>
void test_relational_operators_for_different_values(
    const T& smaller, const T& bigger)
{
    REQUIRE(smaller < bigger);
    CHECK_FALSE(smaller == bigger);
    CHECK(smaller != bigger);
    CHECK(smaller <= bigger);
    CHECK_FALSE(smaller > bigger);
    CHECK_FALSE(smaller >= bigger);
}

template <class T>
void test_relational_operators_noexcept(const T& x)
{
    static_assert(noexcept(x == x));
    static_assert(noexcept(x != x));
    static_assert(noexcept(x < x));
    static_assert(noexcept(x <= x));
    static_assert(noexcept(x > x));
    static_assert(noexcept(x >= x));
}

} // namespace ac