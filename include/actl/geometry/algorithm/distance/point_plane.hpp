// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/distance/distance.hpp>
#include <actl/geometry/algorithm/point/norm.hpp>
#include <actl/geometry/plane.hpp>

namespace ac {

template<index N, class T0, class T1>
auto distance(
    Policy auto const& policy, point<T0, N> const& p, plane<T1, N> const& pl
) {
    AC_ASSERT(!degenerate(pl));
    return ratio(policy, abs(pl(policy, p)), norm(policy, pl.normal));
}

} // namespace ac
