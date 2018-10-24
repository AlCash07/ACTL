/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/dummy_container.hpp>
#include <actl/test.hpp>

using namespace ac;

TEST("basic") {
    dummy_container c(3);
    c.emplace_back(1);
    ASSERT_EQUAL(4, c.size());
    c.erase(0);
    c.erase(1);
    ASSERT_EQUAL(2, c.size());
    c.clear();
    ASSERT_TRUE(c.empty());
    c.resize(3);
    ASSERT_EQUAL(3, c.size());
    ASSERT_FALSE(c.empty());
}
