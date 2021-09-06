// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/array/semi_static_array.hpp>
#include "test_regular.hpp"

using namespace ac::constant_literals;

template <int I>
using int_c_t = std::integral_constant<int, I>;

TEST_CASE("semi_static_array")
{
    using ssa = ac::semi_static_array<int, 2, -1, 3, -1>;
    static_assert(std::is_trivial_v<ssa>);
    static_assert(ac::is_trivially_moveable_v<ssa>);
    static_assert(std::is_standard_layout_v<ssa>);
    ac::test_nothrow_regular_traits<ssa>();
    ac::test_regular(ssa{5, 4}, ssa{4, 4});

    // constructor from std::array
    static_assert(ssa{5, 4} == ssa{std::array{5, 4}});

    SECTION("element access")
    {
        constexpr std::array array{3, 5, 4, 2};
        const ac::semi_static_array<int, 3, -1, -1, 2> a{array};
        static_assert(4 == a.size());
        static_assert(std::is_same_v<int_c_t<3>, decltype(a[0_c])>);
        static_assert(std::is_same_v<int_c_t<2>, decltype(a[3_c])>);
        for (int i = 0; i < a.size(); ++i)
        {
            CHECK(array[(size_t)i] == a[i]);
        }
    }

    SECTION("dynamic element modification")
    {
        ac::semi_static_array<int, 3, -1> mutable_a{2};
        CHECK(2 == mutable_a[1]);
        mutable_a[1_c] = 5;
        CHECK(5 == mutable_a[1]);
    }

    // comparison of different types
    using ssa1 = ac::semi_static_array<int, 2, 3, -1, -1>;
    using ssa2 = ac::semi_static_array<int, 2, -1, 4, -1>;
    static_assert(ssa1{4, 5} == ssa2{3, 5});
    static_assert(ssa1{4, 5} != ac::semi_static_array<int, 1, -1, 4, -1>{3, 5});
    static_assert(ssa1{4, 5} != ssa2{2, 5});
    static_assert(ssa1{2, 5} != ssa2{3, 5});
    static_assert(ssa1{4, 5} != ssa2{3, 6});

    // comparison with static_array
    using ssa3 = ac::semi_static_array<int, 2, -1>;
    static_assert(ssa3{3} == ac::static_array<int, 2, 3>{});
    static_assert(ssa3{3} != ac::static_array<int, 1, 3>{});
    static_assert(ssa3{4} != ac::static_array<int, 2, 3>{});
}
