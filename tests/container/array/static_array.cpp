/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/array/static_array.hpp>
#include <actl/test.hpp>

TEST("empty") {
    constexpr static_array<int> sa{};
    static_assert(0 == sa.size());
}

TEST("contents") {
    constexpr static_array<int, 3, 0, 2> sa{};
    static_assert(3 == sa.size());
    static_assert(3 == sa[0]);
    static_assert(0 == sa[1]);
    static_assert(2 == sa[2]);
}
