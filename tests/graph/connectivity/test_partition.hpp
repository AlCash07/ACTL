// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include "test.hpp"

template<typename T>
void test_partition(
    std::vector<std::vector<size_t>> const& expected, T const& actual
) {
    for (size_t i = 0; i < expected.size(); ++i) {
        for (auto v : expected[i]) {
            CHECK(actual[expected[i][0]] == actual[v]);
        }
        for (size_t j = 0; j < expected.size(); ++j) {
            if (i != j) {
                CHECK(actual[expected[i][0]] != actual[expected[j][0]]);
            }
        }
    }
}
