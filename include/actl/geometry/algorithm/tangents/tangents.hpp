/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/traits.hpp>

namespace ac {

template <class Policy, class T0, class T1, class OutIter, geometry::enable_int_if_swap<T0, T1> = 0>
inline auto tangents(Policy&& policy, const T0& lhs, const T1& rhs, OutIter dst) {
    return tangents(policy, rhs, lhs, dst);
}

template <class T0, class T1, class OutIter>
inline auto tangents(const T0& lhs, const T1& rhs, OutIter dst) {
    return tangents(use_default{}, lhs, rhs, dst);
}

}  // namespace ac
