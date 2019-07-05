/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/project/plane.hpp>
#include <actl/std/utility.hpp>

namespace ac {

template <class P = use_default, class R = use_default, class ProjectPolicy = project_plane<P, R>>
struct nearest_point_plane : ProjectPolicy {};

template <class P, class R, index N, class T0, class T1>
inline auto nearest(const nearest_point_plane<P, R>& policy, const point<T0, N>& point,
                    const plane<T1, N>& plane) {
    return std::pair{point, project(policy, point, plane)};
}

template <index N, class T0, class T1>
inline auto nearest(use_default, const point<T0, N>& point, const plane<T1, N>& plane) {
    return nearest(nearest_point_plane<>{}, point, plane);
}

}  // namespace ac
