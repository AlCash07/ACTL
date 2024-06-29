// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <catch2/catch_test_macros.hpp>
#include <concepts>

namespace ac {

template<class T>
void test_regular_default_constructor() {
    T x;
    CHECK(x == x);
}

template<class T>
void test_regular_copy(T const x, T const y) {
    T z{x};
    CHECK(x == z);
    CHECK(y != z);
    z = y;
    CHECK(y == z);
    CHECK(x != z);
}

template<class T>
void test_regular_move(T x, T y) {
    T x1{x};
    T z{std::move(x1)};
    CHECK(x == z);
    CHECK(y != z);
    z = std::move(y);
    CHECK(y == z);
    CHECK(x != z);
}

template<class T>
void test_regular_swap(T x, T y) {
    T x1{x}, y1{y};
    using std::swap;
    swap(x1, y1);
    CHECK(x == y1);
    CHECK(y == x1);
}

template<class T>
void test_regular(T x, T y) {
    static_assert(std::regular<T>);
    REQUIRE(x != y);
    test_regular_default_constructor<T>();
    test_regular_copy(x, y);
    test_regular_move(x, y);
    test_regular_swap(x, y);
}

} // namespace ac
