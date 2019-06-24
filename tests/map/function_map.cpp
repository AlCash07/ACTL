/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/map/function_map.hpp>
#include <actl/test.hpp>
#include <cstring>

TEST("free_function") {
    auto map = function_map{std::strlen};
    ASSERT_EQUAL(0u, get(map, ""));
    ASSERT_EQUAL(3u, get(map, "map"));
}

TEST("lambda") {
    int count = 0;
    auto map = function_map{[&count](int) { return count++; }};
    ASSERT_EQUAL(0, get(map, 0));
    ASSERT_EQUAL(1, get(map, 0));
    ASSERT_EQUAL(2, count);
}
