// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/policy/absolute_error.hpp>
#include <actl/test.hpp>

constexpr double eps = 1e-9;

struct Eps {
    static constexpr double epsilon() { return eps; }
};

constexpr math::absolute_error<Eps> eps_policy;

TEST("equal") {
    constexpr auto abs_equal = math::equal(eps_policy);
    ASSERT_TRUE(abs_equal(0.0, eps));
    ASSERT_FALSE(abs_equal(0.0, eps + math::sqr(eps)));
}

TEST("less") {
    constexpr auto abs_less = math::less(eps_policy);
    ASSERT_FALSE(math::less(0.0, -eps));
    ASSERT_TRUE(math::less(-eps, 0.0));
    ASSERT_FALSE(abs_less(-eps, 0.0));
    ASSERT_TRUE(abs_less(-eps - math::sqr(eps), 0.0));
}
