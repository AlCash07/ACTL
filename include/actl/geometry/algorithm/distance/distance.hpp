/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/traits.hpp>

namespace ac {

template <class Policy, class T, class U, geometry::enable_int_if_swap<T, U> = 0>
inline auto distance(const Policy& policy, const T& lhs, const U& rhs) {
    return distance(policy, rhs, lhs);
}

template <class T, class U>
inline auto distance(const T& lhs, const U& rhs) {
    return distance(geometry_policy, rhs, lhs);
}

}  // namespace ac
