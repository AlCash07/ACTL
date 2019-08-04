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

template <class Policy, index N, class T0, class T1>
inline auto project(const Policy& policy, const point<T0, N>& p, const plane<T1, N>& pl) {
    auto t = ratio(policy, pl(policy, p), dot(policy, pl.normal));
    return p - product(policy, t, pl.normal);
}

}  // namespace ac
