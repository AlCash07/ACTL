/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at
 *http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/linear_algebra/vector.hpp>
#include <actl/test.hpp>

using namespace ac::math;

#if 0
TEST("arithmetic") {
    vec vi{1, 2};
    vec vf{3.5f, 4.5f};
    vec vd{5.5, 6.5};
    ASSERT_EQUAL(vec{-1, -2}, -vi);
    ASSERT_EQUAL(vec{4.5f, 6.5f}, vi + vf, 1e-9f);
    ASSERT_EQUAL(vec{4.5, 4.5}, vd - vi, 1e-12);
    ASSERT_EQUAL(vec{3, 6}, vi * 3);
    ASSERT_EQUAL(vec{4, 8}, 4 * vi);
    ASSERT_EQUAL(vec{2.75, 3.25}, vd / 2, 1e-12);
    // ASSERT_EQUAL(18.5, dot(vi, vd), 1e-12);
}

TEST("inplace arithmetic") {
    vec v = {1, 2};
    ASSERT_EQUAL(vec{3, 5}, v += {2, 3});
    ASSERT_EQUAL(vec{2, 6}, v -= {1, -1});
    ASSERT_EQUAL(vec{1, 3}, v /= 2);
    ASSERT_EQUAL(vec{3, 9}, v *= 3);
    ASSERT_EQUAL(vec{2, 3}, v = vec<int>{vec{2.5, 3.5}});
}

TEST("comparison") {
    ASSERT_EQUAL(vec{1, 2}, vec{1, 2});
    ASSERT_TRUE(vec{1, 2} > vec{0, 3});
    ASSERT_FALSE(vec{1, 2} > vec{1, 3});
    ASSERT_TRUE(vec{1, 2} <= vec{1, 3});
    ASSERT_FALSE(vec{1, 2} <= vec{0, 1});
}
#endif