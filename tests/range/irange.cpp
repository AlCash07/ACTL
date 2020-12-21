// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/range/algorithm.hpp>
#include <actl/range/irange.hpp>

using VI = std::vector<int>;

TEST_CASE("irange") {
    VI v;
    copy(irange(2, 0), std::back_inserter(v));
    copy(irange(2, 2), std::back_inserter(v));
    CHECK(VI{} == v);
    copy(irange(2, 5), std::back_inserter(v));
    CHECK(VI{2, 3, 4} == v);
    v = {};
    copy(irange(2), std::back_inserter(v));
    CHECK(VI{0, 1} == v);
}

TEST_CASE("drange") {
    VI v;
    copy(drange(2, 4), std::back_inserter(v));
    CHECK(VI{} == v);
    copy(drange(2, 2), std::back_inserter(v));
    CHECK(VI{2} == v);
    copy(drange(-1, -2), std::back_inserter(v));
    CHECK(VI{2, -1, -2} == v);
    v = {};
    copy(drange(2), std::back_inserter(v));
    CHECK(VI{2, 1, 0} == v);
}

TEST_CASE("irange with step > 0") {
    VI v;
    copy(irange(2, 0, 2), std::back_inserter(v));
    copy(irange(2, 2, 2), std::back_inserter(v));
    CHECK(VI{} == v);
    copy(irange(2, 6, 2), std::back_inserter(v));
    CHECK(VI{2, 4} == v);
    v = {};
    copy(irange(-5, -2, 2), std::back_inserter(v));
    CHECK(VI{-5, -3} == v);
}

TEST_CASE("irange with step < 0") {
    VI v;
    copy(irange(2, 4, -2), std::back_inserter(v));
    CHECK(VI{} == v);
    copy(irange(2, 2, -2), std::back_inserter(v));
    CHECK(VI{2} == v);
    copy(irange(5, 3, -2), std::back_inserter(v));
    CHECK(VI{2, 5, 3} == v);
    v = {};
    copy(irange(-2, -5, -2), std::back_inserter(v));
    CHECK(VI{-2, -4} == v);
}
