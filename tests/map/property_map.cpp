/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/map/property_map.hpp>
#include <actl/test.hpp>

TEST("iterator_map") {
    int a[3] = {2, 1, 0};
    int* p = a;
    ASSERT_EQUAL(2, get(p, 0));
    put(p, 2, 4);
    ASSERT_EQUAL(4, a[2]);
}
