/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/point/collinear.hpp>
#include <actl/geometry/plane.hpp>

namespace ac {

template <class CollinearPolicy = comparable_collinear<>>
struct crosses_plane_plane : CollinearPolicy {};

template <class CP, int N, class T0, class T1>
inline bool crosses(const crosses_plane_plane<CP>& policy, const plane<T0, N>& lhs,
                    const plane<T1, N>& rhs) {
    ACTL_ASSERT(lhs && rhs);
    if (!collinear(policy, lhs.normal, rhs.normal)) return true;
    int i = 0;
    while (lhs.normal[i] == T0{0}) ++i;
    return collinear(policy, make_point(lhs.normal[i], lhs.d), make_point(rhs.normal[i], rhs.d));
}

template <int N, class T0, class T1>
inline bool crosses(use_default, const plane<T0, N>& lhs, const plane<T1, N>& rhs) {
    return crosses(crosses_plane_plane<>(), lhs, rhs);
}

}  // namespace ac
