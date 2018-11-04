/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/property_map/logging_property_map.hpp>
#include <actl/test.hpp>
#include <vector>

using namespace ac;

TEST("int*") {
    int data[4];
    using L = std::vector<std::pair<int, int>>;
    L log;
    auto pm = make_logging_property_map<int*>(std::back_inserter(log), data);
    put(pm, 1, 2);
    put(pm, 2, 4);
    put(pm, 1, 3);
    ASSERT_EQUAL(L{{1, 2}, {2, 4}, {1, 3}}, log);
}
