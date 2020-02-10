/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/container/static_array.hpp>
#include <actl/test.hpp>

TEST("empty static_array") {
    constexpr part_static_array<int> sa{};
    static_assert(0 == sa.size());
}

TEST("static_array") {
    constexpr part_static_array<int, 3, 0, 2> sa{};
    static_assert(3 == sa.size());
    static_assert(3 == sa[0]);
    static_assert(0 == sa[1]);
    static_assert(2 == sa[2]);
}

TEST("semi_static_array") {
    std::array a{3, 5, 4, 2};
    part_static_array<int, 3, -1, -1, 2> sa{a};
    static_assert(4 == sa.size());
    for (int i = 0; i < a.size(); ++i) {
        ASSERT_EQUAL(a[(size_t)i], sa[i]);
    }
}
