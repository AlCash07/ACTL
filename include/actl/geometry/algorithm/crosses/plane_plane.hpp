/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/crosses/crosses.hpp>
#include <actl/geometry/algorithm/point/collinear.hpp>
#include <actl/geometry/plane.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1>
inline bool crosses(const Policy& policy, const plane<T0, N>& lhs, const plane<T1, N>& rhs) {
    ACTL_ASSERT(!degenerate(policy, lhs) && !degenerate(policy, rhs));
    if (!collinear(policy, lhs.normal, rhs.normal)) return true;
    index i = 0;
    while (equal(policy, lhs.normal[i], 0)) ++i;
    return collinear(policy, point{lhs.normal[i], lhs.d}, point{rhs.normal[i], rhs.d});
}

}  // namespace ac
