/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/map/logging_map.hpp>
#include <actl/std/vector.hpp>
#include <actl/test.hpp>

TEST("int*") {
    int data[4];
    using Log = std::vector<std::pair<int, int>>;
    Log log;
    auto pm = logging_map{(int*)data, std::back_inserter(log)};
    put(pm, 1, 2);
    put(pm, 2, 4);
    ASSERT_EQUAL(2, get(pm, 1));
    put(pm, 1, 3);
    ASSERT_EQUAL(Log{{1, 2}, {2, 4}, {1, 3}}, log);
}
