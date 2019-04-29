/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/property_map/function_property_map.hpp>
#include <actl/test.hpp>
#include <cstring>

TEST("free_function") {
    auto pm = function_property_map{std::strlen};
    ASSERT_EQUAL(0u, get(pm, ""));
    ASSERT_EQUAL(2u, get(pm, "pm"));
}

TEST("lambda") {
    int count = 0;
    auto pm = function_property_map{[&count](int) { return count++; }};
    ASSERT_EQUAL(0, get(pm, 0));
    ASSERT_EQUAL(1, get(pm, 0));
    ASSERT_EQUAL(2, count);
}
