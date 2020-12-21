// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include <actl/range/traits/range_traits.hpp>

TEST_CASE("is_container") {
    CHECK_FALSE(is_container_v<int>);
    CHECK_FALSE(is_container_v<int*>);
    CHECK_FALSE(is_container_v<int[]>);
    CHECK(is_container_v<int[2]>);
}
