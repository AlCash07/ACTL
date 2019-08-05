/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/orientation/point_point.hpp>
#include <actl/geometry/line.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1, class K, enable_int_if_policy<Policy> = 0>
inline constexpr auto orientation(const Policy& policy, const point<T0, N>& p,
                                  const line<T1, N, K>& l) {
    return orientation(policy, p - l.begin, l.vector);
}

}  // namespace ac
