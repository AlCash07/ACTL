// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/geometry/algorithm/point/norm.hpp>
#include "test.hpp"

TEST_CASE("default") {
    CHECK(1.414213562373095, norm(point{1, 1}) == 1e-12);
}

struct norm_policy
    : virtual cast_before<Mul, long long>
    , virtual defer_sqrt {};

TEST_CASE("deferred") {
    constexpr int x = 2'000'000'000;
    CHECK(8'000'000'000'000'000'000LL == sqr(norm(norm_policy{}, point{x, -x})));
}
