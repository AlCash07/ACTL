// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <catch2/catch_test_macros.hpp>
#include <concepts>

namespace ac {

template<typename T>
void test_regular_default_constructor() {
    T t;
    CHECK(t == t);
}

template<typename T>
void test_regular_copy(T const l, T const r) {
    T t{l};
    CHECK(l == t);
    CHECK(r != t);
    t = r;
    CHECK(r == t);
    CHECK(l != t);
}

template<typename T>
void test_regular_move(T l, T r) {
    T l1{l};
    T t{std::move(l1)};
    CHECK(l == t);
    CHECK(r != t);
    t = std::move(r);
    CHECK(r == t);
    CHECK(l != t);
}

template<typename T>
void test_regular_swap(T l, T r) {
    T l1{l}, r1{r};
    using std::swap;
    swap(l1, r1);
    CHECK(l == r1);
    CHECK(r == l1);
}

template<typename T>
void test_regular(T l, T r) {
    static_assert(std::regular<T>);
    REQUIRE(l != r);
    test_regular_default_constructor<T>();
    test_regular_copy(l, r);
    test_regular_move(l, r);
    test_regular_swap(l, r);
}

} // namespace ac
