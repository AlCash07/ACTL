// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/array/static_array.hpp>
#include <actl/integral_constant.hpp>
#include <actl_test/base/concept/regular.hpp>

using namespace ac::constant_literals;

template <int I>
using int_c_t = std::integral_constant<int, I>;

TEST_CASE("static_array")
{
    using sa = ac::static_array<int, 1, 3>;
    static_assert(std::is_standard_layout_v<sa>);
    ac::test_trivially_semiregular_type_traits<sa>();
    ac::test_nothrow_regular_type_traits<sa>();

    // size
    static_assert(0 == ac::static_array<int>{}.size());

    constexpr ac::static_array<int, 3, 0, 2> a{};
    static_assert(3 == a.size());

    // element access
    static_assert(3 == a[0]);
    static_assert(std::is_same_v<int_c_t<3>, decltype(a[0_c])>);
    static_assert(0 == a[1]);
    static_assert(std::is_same_v<int_c_t<0>, decltype(a[1_c])>);
    static_assert(2 == a[2]);
    static_assert(std::is_same_v<int_c_t<2>, decltype(a[2_c])>);
}
