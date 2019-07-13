/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/project/line.hpp>
#include <actl/std/utility.hpp>

namespace ac {

template <class P = use_default, class F = use_default, class ProjectPolicy = project_line<P, F>>
struct nearest_point_line : ProjectPolicy {};

template <class P, class F, index N, class T0, class T1, class K,
          class X = geometry::float_t<F, T0, T1>>
inline std::pair<point<T0, N>, point<X, N>> nearest(const nearest_point_line<P, F>& policy,
                                                    const point<T0, N>& point,
                                                    const line<T1, N, K>& line) {
    if (line.start_kind() != endpoint::free && dot<P>(point - line.start, line.vector) <= 0)
        return {point, line.start};
    if (line.end_kind() != endpoint::free && dot<P>(point - line.end(), line.vector) >= 0)
        return {point, line.end()};
    return {point, project(policy, point, line)};
}

template <index N, class T0, class T1, class K>
inline auto nearest(use_default, const point<T0, N>& point, const line<T1, N, K>& line) {
    return nearest(nearest_point_line<>{}, point, line);
}

}  // namespace ac
