/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/plane.hpp>
#include <actl/geometry/traits/policy.hpp>

namespace ac {

template <class P = use_default>
struct within_plane : geometry::policy {};

template <class P, int N, class T0, class T1>
inline int within(within_plane<P>, const point<N, T0>& point, const plane<N, T1>& plane) {
    return plane.template operator()<P>(point) == 0 ? 0 : 1;
}

template <int N, class T0, class T1>
inline int within(use_default, const point<N, T0>& point, const plane<N, T1>& plane) {
    return within(within_plane<>(), point, plane);
}

}  // namespace ac
