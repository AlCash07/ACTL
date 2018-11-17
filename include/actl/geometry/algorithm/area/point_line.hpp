/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/area/point_point.hpp>
#include <actl/geometry/line.hpp>

namespace ac {

template <class Policy, int N, class T0, class T1, class K>
inline auto area(const Policy& policy, const point<T0, N>& point, const line<T1, N, K>& line) {
    return area(policy, point - line.start, line.vector);
}

}  // namespace ac
