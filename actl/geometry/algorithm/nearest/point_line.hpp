/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/project/line.hpp>
#include <utility>

namespace ac {

template <class P = use_default, class R = use_default, class ProjectPolicy = project_line<P, R>>
struct nearest_point_line : ProjectPolicy {};

template <class P, class R, int N, class T0, class T1, class K,
          class X = geometry::ratio_t<R, T0, T1>>
inline std::pair<point<T0, N>, point<X, N>> nearest(const nearest_point_line<P, R>& policy,
                                                    const point<T0, N>& point,
                                                    const line<T1, N, K>& line) {
    if (line.start_kind() != endpoint::free && dot<P>(point - line.start, line.slope) <= 0)
        return {point, line.start};
    if (line.end_kind() != endpoint::free && dot<P>(point - line.end(), line.slope) >= 0)
        return {point, line.end()};
    return {point, project(policy, point, line)};
}

template <int N, class T0, class T1, class K>
inline auto nearest(use_default, const point<T0, N>& point, const line<T1, N, K>& line) {
    return nearest(nearest_point_line<>(), point, line);
}

}  // namespace ac
