// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/array/static_array.hpp>
#include "test_regular.hpp"

TEST_CASE("static_array")
{
    using sa = ac::static_array<int, 1, 3>;
    static_assert(std::is_trivial_v<sa>);
    static_assert(ac::is_trivially_moveable_v<sa>);
    static_assert(std::is_standard_layout_v<sa>);
    ac::test_nothrow_regular_traits<sa>();

    // empty
    static_assert(0 == ac::static_array<int>{}.size());

    // contents
    constexpr ac::static_array<int, 3, 0, 2> a{};
    static_assert(3 == a.size());
    static_assert(3 == a[0]);
    static_assert(0 == a[1]);
    static_assert(2 == a[2]);
}
