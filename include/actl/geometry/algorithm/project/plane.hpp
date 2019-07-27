/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/plane.hpp>

namespace ac {

template <class P = use_default, class F = use_default>
struct project_plane : geometry::policy {};

template <class P, class F, index N, class T0, class T1, class X = geometry::float_t<F, T0, T1>>
inline auto project(project_plane<P, F> policy, const point<T0, N>& src, const plane<T1, N>& dst) {
    return src - dst.normal * static_cast<X>(dst(policy, src)) / dot(policy, dst.normal);
}

template <index N, class T0, class T1>
inline auto project(use_default, const point<T0, N>& src, const plane<T1, N>& dst) {
    return project(project_plane{}, src, dst);
}

}  // namespace ac
