/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/tensor.hpp>
#include <actl/test.hpp>

TEST("rank 0 initialization") {
    tensor<int, 0> t_init{2};
    ASSERT_EQUAL(1l, t_init.size());
    // TODO: make this work again.
    // ASSERT_EQUAL(2, t_init);
}

TEST("rank 1 initialization") {
    tensor<int, 1> t_dims(2);
    ASSERT_EQUAL(std::vector{2}, t_dims.dimensions());
    tensor<int, 1> t_init{2};
    ASSERT_EQUAL(std::vector{1}, t_init.dimensions());
}

TEST("rank 3 initialization") {
    tensor<int, 3> t_dims{{5, 4, 3}, 0};
    ASSERT_EQUAL(std::vector{5, 4, 3}, t_dims.dimensions());
    for (auto x : t_dims) {
        ASSERT_EQUAL(0, x);
    }
    tensor<int, 3> t_init{{{1, 2, 3}}, {{4}, {5, 6}}};
    ASSERT_EQUAL(std::vector{2, 2, 3}, t_init.dimensions());
    ASSERT_EQUAL(std::vector{1, 2, 3, 0, 0, 0, 4, 0, 0, 5, 6, 0}, t_init);
}

TEST("indexing") {
    tensor<int, 0> t0{2};
    ASSERT_EQUAL(t0(), 2);
    tensor<int, 3> t3{{{1, 2, 3}}, {{4}, {5, 6}}};
    ASSERT_EQUAL(3, t3(0, 0, 2));
    ASSERT_EQUAL(4, t3(1, 0, 0));
    ASSERT_EQUAL(6, t3(1, 1, 1));
}

TEST("rank -1") {
    tensor<int> t{{3, 2}};
    ASSERT_EQUAL(std::vector{3, 2}, t.dimensions());
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) t(i, j) = i * 2 + j;
    }
    ASSERT_EQUAL(std::vector{0, 1, 2, 3, 4, 5}, span{t});
}
