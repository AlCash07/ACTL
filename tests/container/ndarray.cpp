/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/ndarray.hpp>
#include <actl/test.hpp>

TEST("rank 0 initialization") {
    ndarray<int, 0> t_init{2};
    ASSERT_EQUAL(t_init, 2);
}

TEST("rank 1 initialization") {
    ndarray<int, 1> t_dims{2, 3, 4};
    ASSERT_EQUAL(3, t_dims.size());
}

TEST("rank 3 initialization") {
    ndarray<int, 3> t_dims{{5, 4, 3}};
    for (auto x : t_dims) {
        ASSERT_EQUAL(0, x);
    }
    ndarray<int, 3> t_init{{{1, 2, 3}}, {{4}, {5, 6}}};
    ASSERT_EQUAL_RANGES(std::vector{1, 2, 3, 0, 0, 0, 4, 0, 0, 5, 6, 0}, t_init);
}
