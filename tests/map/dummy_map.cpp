/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/map/dummy_map.hpp>
#include <actl/test.hpp>

TEST("basic") {
    dummy_map<int, int> map;
    put(map, 2, 2);
    ASSERT_EQUAL(0, get(map, 2));
}
