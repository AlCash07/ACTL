// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/point/norm.hpp>

namespace ac {

template<index N, typename T0, typename T1>
auto bisector(
    Policy auto const& policy, point<T0, N> const& lhs, point<T1, N> const& rhs
) {
    return product(policy, lhs, norm(policy, rhs)) +
           product(policy, rhs, norm(policy, lhs));
}

template<index N, typename T0, typename T1>
auto bisector(point<T0, N> const& lhs, point<T1, N> const& rhs) {
    return bisector(geometry_policy, lhs, rhs);
}

} // namespace ac
