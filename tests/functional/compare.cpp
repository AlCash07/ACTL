/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/functional/compare.hpp>
#include <actl/test.hpp>
#include <limits>

constexpr double eps = 1e-9;

struct Eps {
    static constexpr double epsilon() { return eps; }
};

inline constexpr absolute_error<Eps> eps_policy;

TEST("op::equal") {
    ASSERT_TRUE(op::equal(eps_policy, 0.0, eps / 2));
    ASSERT_FALSE(op::equal(eps_policy, 0.0, eps));
}

TEST("op::less") {
    ASSERT_TRUE(op::less(use_default{}, 0.0, eps));
    ASSERT_FALSE(op::less(eps_policy, 0.0, eps));
    ASSERT_TRUE(op::less(eps_policy, -eps, eps));
}

TEST("sgn") {
    ASSERT_EQUAL(1, sgn(std::numeric_limits<int>::max()));
    ASSERT_EQUAL(-1, sgn(std::numeric_limits<int>::min()));
    ASSERT_EQUAL(1, sgn(std::numeric_limits<unsigned int>::max()));
    ASSERT_EQUAL(0, sgn(0));
    ASSERT_EQUAL(1, sgn(0, -1));
    ASSERT_EQUAL(-1, sgn(0u, 1u));
}

TEST("smax") {
    int x = 4;
    ASSERT_EQUAL(5, smax(x, 5));
    ASSERT_EQUAL(5, x);
    ASSERT_EQUAL(5, smax(x, 3));
    ASSERT_EQUAL(5, x);
}

TEST("smin") {
    int x = 4;
    ASSERT_EQUAL(4, smin(x, 5));
    ASSERT_EQUAL(4, x);
    ASSERT_EQUAL(3, smin(x, 3));
    ASSERT_EQUAL(3, x);
}
