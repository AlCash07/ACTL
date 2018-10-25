/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/test.hpp>
#include <actl/traits/nth_type.hpp>

using namespace ac;

TEST("correctness") {
    ASSERT_TRUE(std::is_same_v<int, nth_type_t<0, int, float>>);
    ASSERT_TRUE(std::is_same_v<float, nth_type_t<1, int, float>>);
}
