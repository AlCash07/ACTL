// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/algorithm.hpp>
#include <actl/range/irange.hpp>
#include <actl/std/array.hpp>
#include "test.hpp"

TEST_CASE("irange") {
    CHECK(ac::irange(2, 0).empty());
    CHECK(ac::irange(2, 2).empty());
    CHECK_EQUAL(std::array{2, 3, 4}, ac::irange(2, 5));
    CHECK_EQUAL(std::array{0, 1}, ac::irange(2));
}

TEST_CASE("drange") {
    CHECK(ac::drange(2, 4).empty());
    CHECK_EQUAL(std::array{2}, ac::drange(2, 2));
    CHECK_EQUAL(std::array{-1, -2}, ac::drange(-1, -2));
    CHECK_EQUAL(std::array{2, 1, 0}, ac::drange(2));
}

TEST_CASE("irange with step > 0") {
    CHECK(ac::irange(2, 0, 2).empty());
    CHECK(ac::irange(2, 2, 2).empty());
    CHECK_EQUAL(std::array{2, 4}, ac::irange(2, 6, 2));
    CHECK_EQUAL(std::array{-5, -3}, ac::irange(-5, -2, 2));
}

TEST_CASE("irange with step < 0") {
    CHECK(ac::irange(2, 4, -2).empty());
    CHECK_EQUAL(std::array{2}, ac::irange(2, 2, -2));
    CHECK_EQUAL(std::array{5, 3}, ac::irange(5, 3, -2));
    CHECK_EQUAL(std::array{-2, -4}, ac::irange(-2, -5, -2));
}
