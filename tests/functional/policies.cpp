/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/functional/policies.hpp>
#include <actl/test.hpp>

constexpr double eps = 1e-9;

struct Eps {
    static constexpr double epsilon() { return eps; }
};

inline op::absolute_error<Eps> eps_policy;

TEST("op::equal") {
    ASSERT_TRUE(op::equal(eps_policy, 0.0, eps / 2));
    ASSERT_FALSE(op::equal(eps_policy, 0.0, eps));
}

TEST("op::less") {
    ASSERT_FALSE(op::less(0.0, -eps));
    ASSERT_TRUE(op::less(-eps / 2, 0.0));
    ASSERT_FALSE(op::less(eps_policy, -eps / 2, 0.0));
    ASSERT_TRUE(op::less(eps_policy, -eps, 0.0));
}
