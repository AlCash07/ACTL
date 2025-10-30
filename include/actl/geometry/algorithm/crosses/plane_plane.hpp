// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/crosses/crosses.hpp>
#include <actl/geometry/algorithm/orientation/point_point.hpp>
#include <actl/geometry/plane.hpp>

namespace ac {

template<index N, typename TL, typename TR>
bool crosses(
    Policy auto const& policy, plane<TL, N> const& lhs, plane<TR, N> const& rhs
) {
    AC_ASSERT(!degenerate(policy, lhs) && !degenerate(policy, rhs));
    if (!collinear(policy, lhs.normal, rhs.normal))
        return true;
    index i = 0;
    while (equal(policy, lhs.normal[i], 0))
        ++i;
    return collinear(
        policy, point{lhs.normal[i], lhs.d}, point{rhs.normal[i], rhs.d}
    );
}

} // namespace ac
