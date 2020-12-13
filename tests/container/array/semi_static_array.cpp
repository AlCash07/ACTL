// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/container/array/semi_static_array.hpp>

#include "regular.hpp"

TEST("regular") {
    using ssa = semi_static_array<int, 2, -1, 3, -1>;
    test_regular_type(ssa{5, 4}, ssa{4, 4});
}

TEST("contents") {
    std::array a{3, 5, 4, 2};
    semi_static_array<int, 3, -1, -1, 2> sa{a};
    static_assert(4 == sa.size());
    for (int i = 0; i < a.size(); ++i) {
        ASSERT_EQUAL(a[(size_t)i], sa[i]);
    }
}
