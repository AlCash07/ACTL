/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

/**
 * Point inclusion test, returns 0 if outside, 1 if on the border, 2 if inside.
 */
template <index N, class T, class U>
inline int within(const point<T, N>& lhs, const U& rhs) {
    return within(geometry_policy, lhs, rhs);
}

}  // namespace ac
