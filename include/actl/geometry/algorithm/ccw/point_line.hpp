/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/ccw/point_point.hpp>
#include <actl/geometry/line.hpp>

namespace ac {

template <class Policy, class T0, class T1, class K, enable_int_if_policy<Policy> = 0>
inline constexpr int ccw(const Policy& policy, const point<T0>& p, const line<T1, 2, K>& l) {
    return ccw(policy, p - l.begin, l.vector);
}

}  // namespace ac
