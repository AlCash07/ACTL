// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/array/semi_static_array.hpp>
#include "regular.hpp"

TEST_CASE("semi_static_array")
{
    using ssa = semi_static_array<int, 2, -1, 3, -1>;
    static_assert(std::is_standard_layout_v<ssa>);
    // test_regular_type(ssa{5, 4}, ssa{4, 4});

    SECTION("contents")
    {
        constexpr std::array array{3, 5, 4, 2};
        const semi_static_array<int, 3, -1, -1, 2> a{array};
        static_assert(4 == a.size());
        for (int i = 0; i < a.size(); ++i)
        {
            CHECK(array[(size_t)i] == a[i]);
        }
    }
}
