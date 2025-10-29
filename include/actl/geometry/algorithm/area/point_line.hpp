// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>
#include <actl/geometry/line.hpp>

namespace ac {

template<index N, typename T0, typename T1, typename K>
auto area(
    Policy auto const& policy, point<T0, N> const& p, line<T1, N, K> const& l
) {
    return area(policy, p - l.begin, l.vector);
}

} // namespace ac
