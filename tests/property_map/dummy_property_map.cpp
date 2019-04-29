/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/property_map/dummy_property_map.hpp>
#include <actl/test.hpp>

TEST("int") {
    dummy_property_map<int, int> pm;
    put(pm, 2, 2);
    ASSERT_EQUAL(0, get(pm, 2));
}

TEST("void") {
    dummy_property_map<int> pm;
    get(pm, 1);
}
