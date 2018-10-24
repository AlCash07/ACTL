/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/range/algorithm.hpp>
#include <actl/range/filtered_range.hpp>
#include <actl/test.hpp>
#include <vector>

using namespace ac;

TEST("correctness") {
    std::vector<int> v{1, 2, 4, 5, 7, 10}, even;
    copy(filter_range(v, [](int x) { return x % 2 == 0; }), std::back_inserter(even));
    ASSERT_EQUAL(std::vector<int>{2, 4, 10}, even);
}
