/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/project/plane.hpp>
#include <utility>

namespace ac {

template <class P = use_default, class R = use_default, class ProjectPolicy = project_plane<P, R>>
struct nearest_point_plane : ProjectPolicy {};

template <class P, class R, int N, class T0, class T1>
inline auto nearest(const nearest_point_plane<P, R>& policy, const point<N, T0>& point,
                    const plane<N, T1>& plane) {
    return std::make_pair(point, project(policy, point, plane));
}

template <int N, class T0, class T1>
inline auto nearest(use_default, const point<N, T0>& point, const plane<N, T1>& plane) {
    return nearest(nearest_point_plane<>(), point, plane);
}

}  // namespace ac
