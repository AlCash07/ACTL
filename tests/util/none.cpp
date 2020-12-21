// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/util/none.hpp>
#include <type_traits>

TEST_CASE("is_empty") {
    CHECK(std::is_empty_v<none>);
}

template <class E, class T>
constexpr bool test_replace_void_v = std::is_same_v<E, replace_void_t<T>>;

TEST_CASE("replace_void") {
    CHECK(test_replace_void_v<int, int>);
    CHECK(test_replace_void_v<none, void>);
    CHECK(test_replace_void_v<const none, const void>);
}
