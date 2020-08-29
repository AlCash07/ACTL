/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/functional/policies.hpp>
#include <actl/test.hpp>

TEST("allow_promotion") {
    static_assert(3LL == math::add(math::allow_promotion{})(1, 2LL));
}

#if 0
constexpr double eps = 1e-9;

struct Eps {
    static constexpr double epsilon() { return eps; }
};

inline math::absolute_error<Eps> eps_policy;

TEST("math::equal") {
    ASSERT_TRUE(eval(math::equal(0.0, eps / 2), eps_policy));
    ASSERT_FALSE(eval(math::equal(0.0, eps), eps_policy));
}

TEST("math::less") {
    ASSERT_FALSE(math::less(0.0, -eps));
    ASSERT_TRUE(math::less(-eps / 2, 0.0));
    ASSERT_FALSE(eval(math::less(-eps / 2, 0.0), eps_policy));
    ASSERT_TRUE(eval(math::less(-eps, 0.0), eps_policy));
}
#endif
