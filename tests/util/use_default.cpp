/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/test.hpp>
#include <actl/util/use_default.hpp>

TEST("deduce_type") {
    ASSERT_TRUE(std::is_same_v<double, deduce_type_t<double, int>>);
    ASSERT_TRUE(std::is_same_v<int, deduce_type_t<use_default, int>>);
}
