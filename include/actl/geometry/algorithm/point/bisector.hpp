// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/point/norm.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1>
auto bisector(
    const Policy& policy, const point<T0, N>& lhs, const point<T1, N>& rhs) //
{
    return product(policy, lhs, norm(policy, rhs)) +
           product(policy, rhs, norm(policy, lhs));
}

template <index N, class T0, class T1>
auto bisector(const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return bisector(geometry_policy, lhs, rhs);
}

} // namespace ac
