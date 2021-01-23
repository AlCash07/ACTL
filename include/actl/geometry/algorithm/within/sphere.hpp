// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/distance/point_point.hpp>
#include <actl/geometry/algorithm/within/within.hpp>
#include <actl/geometry/sphere.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1>
enum within within(const Policy& policy, const point<T0, N>& p, const sphere<T1, N>& s) {
    return static_cast<enum within>(1 + cmp3way(policy, distance(policy, p, s.center), s.radius));
}

}  // namespace ac
