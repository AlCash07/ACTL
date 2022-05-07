// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/container/conversion/convert_to.hpp>
#include <actl_test/base/equal_same_type.hpp>

/* arithmetic conversions are supported */
static_assert(ac::can_convert_to_v<int, long long>);
static_assert(ac::equal_same_type(2, ac::convert_to<int>(2LL)));

struct P3
{
    int x, y, z;

    constexpr bool operator==(P3 const& rhs) const noexcept
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
};

/* multiple arguments and aggregate initialization are supported */
static_assert(ac::can_convert_to_v<P3, int, int, int>);
static_assert(ac::equal_same_type(P3{5, 4, 2}, ac::convert_to<P3>(5, 4, 2)));
