/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/line.hpp>

namespace ac {

template <class P = use_default, class R = use_default>
struct project_line : geometry::policy {};

template <class P = use_default, class R = use_default>
struct project_line_scalar : geometry::policy {};

template <class P, class R, index N, class T0, class T1, class K>
inline auto project(project_line_scalar<P, R>, const point<T0, N>& src, const line<T1, N, K>& dst) {
    return static_cast<geometry::ratio_t<R, T0, T1>>(dot<P>(src - dst.start, dst.vector)) /
           abs<P>(dst.vector);
}

template <class P, class R, index N, class T0, class T1, class K>
inline auto project(project_line<P, R>, const point<T0, N>& src, const line<T1, N, K>& dst) {
    return dst(project(project_line_scalar<P, R>{}, src, dst));
}

template <index N, class T0, class T1, class K>
inline auto project(use_default, const point<T0, N>& src, const line<T1, N, K>& dst) {
    return project(project_line<>{}, src, dst);
}

}  // namespace ac
