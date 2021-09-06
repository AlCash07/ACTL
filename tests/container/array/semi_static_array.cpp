// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/array/semi_static_array.hpp>
#include "test_regular.hpp"

TEST_CASE("semi_static_array")
{
    using ssa = ac::semi_static_array<int, 2, -1, 3, -1>;
    static_assert(std::is_trivial_v<ssa>);
    static_assert(ac::is_trivially_moveable_v<ssa>);
    static_assert(std::is_standard_layout_v<ssa>);
    ac::test_nothrow_regular_traits<ssa>();
    ac::test_regular(ssa{5, 4}, ssa{4, 4});

    SECTION("contents")
    {
        constexpr std::array array{3, 5, 4, 2};
        const ac::semi_static_array<int, 3, -1, -1, 2> a{array};
        static_assert(4 == a.size());
        for (int i = 0; i < a.size(); ++i)
        {
            CHECK(array[(size_t)i] == a[i]);
        }
    }

    // comparison of different types
    static_assert(
        ac::semi_static_array<int, 2, 3, -1, -1>{4, 5} ==
        ac::semi_static_array<int, 2, -1, 4, -1>{3, 5});
    static_assert(
        ac::semi_static_array<int, 2, 3, -1, -1>{4, 5} !=
        ac::semi_static_array<int, 1, -1, 4, -1>{3, 5});
    static_assert(
        ac::semi_static_array<int, 2, 3, -1, -1>{4, 5} !=
        ac::semi_static_array<int, 2, -1, 4, -1>{2, 5});
    static_assert(
        ac::semi_static_array<int, 2, 3, -1, -1>{2, 5} !=
        ac::semi_static_array<int, 2, -1, 4, -1>{3, 5});
    static_assert(
        ac::semi_static_array<int, 2, 3, -1, -1>{4, 5} !=
        ac::semi_static_array<int, 2, -1, 4, -1>{3, 6});
}
