/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/line.hpp>

namespace ac {

template <class P = use_default, class F = use_default>
struct project_line : geometry::policy {};

template <class P = use_default, class F = use_default>
struct project_line_scalar : geometry::policy {};

template <class P, class F, index N, class T0, class T1, class K>
inline auto project(project_line_scalar<P, F> policy, const point<T0, N>& src,
                    const line<T1, N, K>& dst) {
    return static_cast<geometry::float_t<F, T0, T1>>(dot(policy, src - dst.start, dst.vector)) /
           dot(policy, dst.vector);
}

template <class P, class F, index N, class T0, class T1, class K>
inline auto project(project_line<P, F>, const point<T0, N>& src, const line<T1, N, K>& dst) {
    return dst(project(project_line_scalar<P, F>{}, src, dst));
}

template <index N, class T0, class T1, class K>
inline auto project(use_default, const point<T0, N>& src, const line<T1, N, K>& dst) {
    return project(project_line{}, src, dst);
}

}  // namespace ac
