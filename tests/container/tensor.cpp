// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/container/tensor.hpp>

TEST_CASE("rank 0 initialization") {
    tensor<int, 0> t_init{2};
    CHECK(1l == t_init.size());
    // TODO: make this work again.
    // CHECK(2 == t_init);
}

TEST_CASE("rank 1 initialization") {
    tensor<int, 1> t_dims(2);
    CHECK_EQUAL(std::vector{2}, t_dims.dimensions());
    tensor<int, 1> t_init{2};
    CHECK_EQUAL(std::vector{1}, t_init.dimensions());
}

TEST_CASE("rank 3 initialization") {
    tensor<int, 3> t_dims{{5, 4, 3}, 0};
    CHECK_EQUAL(std::vector{5, 4, 3}, t_dims.dimensions());
    for (auto x : t_dims) {
        CHECK(0 == x);
    }
    tensor<int, 3> t_init{{{1, 2, 3}}, {{4}, {5, 6}}};
    CHECK_EQUAL(std::vector{2, 2, 3}, t_init.dimensions());
    CHECK_EQUAL(std::vector{1, 2, 3, 0, 0, 0, 4, 0, 0, 5, 6, 0}, t_init);
}

TEST_CASE("indexing") {
    tensor<int, 0> t0{2};
    CHECK(t0() == 2);
    tensor<int, 3> t3{{{1, 2, 3}}, {{4}, {5, 6}}};
    CHECK(3 == t3(0, 0, 2));
    CHECK(4 == t3(1, 0, 0));
    CHECK(6 == t3(1, 1, 1));
}

TEST_CASE("rank -1") {
    tensor<int> t{{3, 2}};
    CHECK(std::vector{3, 2} == t.dimensions());
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j)
            t(i, j) = i * 2 + j;
    }
    CHECK_EQUAL(std::vector{0, 1, 2, 3, 4, 5}, span{t});
}
