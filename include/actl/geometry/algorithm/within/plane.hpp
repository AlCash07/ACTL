/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/plane.hpp>

namespace ac {

template <class P = use_default>
struct within_plane : geometry::policy {};

template <class P, index N, class T0, class T1>
inline int within(within_plane<P> policy, const point<T0, N>& p, const plane<T1, N>& pl) {
    return pl(policy, p) == 0;
}

template <index N, class T0, class T1>
inline int within(use_default, const point<T0, N>& point, const plane<T1, N>& plane) {
    return within(within_plane{}, point, plane);
}

}  // namespace ac
