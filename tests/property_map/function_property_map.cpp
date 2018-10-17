/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/property_map/function_property_map.hpp>
#include <actl/test.hpp>
#include <cstring>

using namespace ac;

TEST("function_property_map::free_function") {
    auto fpm = make_function_property_map(std::strlen);
    ASSERT_EQUAL(0, get(fpm, ""));
    ASSERT_EQUAL(3, get(fpm, "fpm"));
}

TEST("function_property_map::lambda") {
    int count = 0;
    auto fpm = make_function_property_map([&count](int) { return count++; });
    ASSERT_EQUAL(0, get(fpm, 0));
    ASSERT_EQUAL(1, get(fpm, 0));
    ASSERT_EQUAL(2, count);
}
