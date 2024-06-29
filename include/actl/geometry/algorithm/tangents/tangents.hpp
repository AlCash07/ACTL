// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/traits.hpp>

namespace ac {

template <class T, class U, class OutIter>
    requires geometry::reverse_order<T, U>
OutIter tangents(
    Policy auto const& policy, T const& lhs, U const& rhs, OutIter dst
) {
    return tangents(policy, rhs, lhs, dst);
}

template <class T, class U, class OutIter>
OutIter tangents(T const& lhs, U const& rhs, OutIter dst) {
    return tangents(geometry_policy, lhs, rhs, dst);
}

} // namespace ac
