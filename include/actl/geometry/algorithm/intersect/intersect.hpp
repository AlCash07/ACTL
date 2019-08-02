/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/traits.hpp>

namespace ac {

template <class Policy, class T, class U, class OutIter, geometry::enable_int_if_swap<T, U> = 0>
inline auto intersect(const Policy& policy, const T& lhs, const U& rhs, OutIter dst) {
    return intersect(policy, rhs, lhs, dst);
}

template <class T, class U, class OutIter>
inline auto intersect(const T& lhs, const U& rhs, OutIter dst) {
    return intersect(geometry_policy, lhs, rhs, dst);
}

}  // namespace ac
