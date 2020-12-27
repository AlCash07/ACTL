// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/functional/policy/absolute_error.hpp>

constexpr double eps = 1e-9;

struct Eps {
    static constexpr double epsilon() {
        return eps;
    }
};

constexpr ac::absolute_error<Eps> eps_policy;

TEST_CASE("equal") {
    constexpr auto abs_equal = ac::equal(eps_policy);
    CHECK(abs_equal(0.0, eps));
    CHECK_FALSE(abs_equal(0.0, eps + ac::sqr(eps)));
}

TEST_CASE("less") {
    constexpr auto abs_less = ac::less(eps_policy);
    CHECK_FALSE(ac::less(0.0, -eps));
    CHECK(ac::less(-eps, 0.0));
    CHECK_FALSE(abs_less(-eps, 0.0));
    CHECK(abs_less(-eps - ac::sqr(eps), 0.0));
}
