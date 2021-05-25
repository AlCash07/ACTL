// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/range/algorithm.hpp>
#include <actl/range/filtered_range.hpp>
#include <actl/std/vector.hpp>
#include "test.hpp"

TEST_CASE("filtered_range correctness")
{
    std::vector<int> v{1, 2, 4, 5, 7, 10};
    CHECK_EQUAL(
        std::vector{2, 4, 10},
        ac::filter_range(v, [](int x) { return x % 2 == 0; }));
}
