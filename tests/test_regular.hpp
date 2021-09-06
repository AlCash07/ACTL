// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include "test.hpp"

namespace ac {

template <class T>
void test_regular_traits()
{
    static_assert(std::is_object_v<T>);
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_destructible_v<T>);
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_move_assignable_v<T>);
    static_assert(std::is_swappable_v<T>);
    static_assert(ac::is_equality_comparable_v<T>);
}

template <class T>
void test_nothrow_regular_traits()
{
    static_assert(std::is_object_v<T>);
    static_assert(std::is_nothrow_default_constructible_v<T>);
    static_assert(std::is_nothrow_destructible_v<T>);
    static_assert(std::is_nothrow_copy_constructible_v<T>);
    static_assert(std::is_nothrow_copy_assignable_v<T>);
    static_assert(std::is_nothrow_move_constructible_v<T>);
    static_assert(std::is_nothrow_move_assignable_v<T>);
    static_assert(std::is_nothrow_swappable_v<T>);
    static_assert(noexcept(
        std::declval<T>() == std::declval<T>(),
        std::declval<T>() != std::declval<T>()));
}

template <class T>
void test_regular_default_constructor()
{
    T x;
    CHECK(x == x);
}

template <class T>
void test_regular_copy(const T x, const T y)
{
    T z{x};
    CHECK(x == z);
    CHECK(y != z);
    z = y;
    CHECK(y == z);
    CHECK(x != z);
}

template <class T>
void test_regular_move(T x, T y)
{
    T x1{x}, y1{y};
    T z{std::move(x1)};
    CHECK(x == z);
    CHECK(y != z);
    z = std::move(y);
    CHECK(y == z);
    CHECK(x != z);
}

template <class T>
void test_regular_swap(T x, T y)
{
    T x1{x}, y1{y};
    using std::swap;
    swap(x1, y1);
    CHECK(x == y1);
    CHECK(y == x1);
}

template <class T>
void test_regular(T x, T y)
{
    test_regular_traits<T>();
    REQUIRE(x != y);
    test_regular_default_constructor<T>();
    test_regular_copy(x, y);
    test_regular_move(x, y);
    test_regular_swap(x, y);
}

} // namespace ac
