// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/geometry/algorithm/distance/distance.hpp>
#include <actl/geometry/algorithm/point/norm.hpp>
#include <actl/geometry/plane.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1>
inline auto distance(const Policy& policy, const point<T0, N>& p, const plane<T1, N>& pl) {
    ACTL_ASSERT(!degenerate(pl));
    return ratio(policy, math::abs(pl(policy, p)), norm(policy, pl.normal));
}

}  // namespace ac
