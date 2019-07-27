/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/project/project.hpp>
#include <actl/geometry/plane.hpp>

namespace ac {

template <class P = use_default, class F = use_default>
struct project_plane : geometry::policy {};

template <class Policy, index N, class T0, class T1>
inline auto project(Policy&& policy, const point<T0, N>& src, const plane<T1, N>& dst) {
    auto t = ratio(policy, dst(policy, src), dot(policy, dst.normal));
    return src - product(policy, t, dst.normal);
}

}  // namespace ac
