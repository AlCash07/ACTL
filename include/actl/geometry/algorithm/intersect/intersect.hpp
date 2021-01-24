// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/detail/scalar_to_point_adaptor.hpp>
#include <actl/geometry/line.hpp>
#include <actl/geometry/traits.hpp>

namespace ac {

template <class Policy, class T, class U, class OutIter, geometry::enable_int_if_swap<T, U> = 0>
OutIter intersect(const Policy& policy, const T& lhs, const U& rhs, OutIter dst) {
    return intersect(policy, rhs, lhs, dst);
}

template <class Policy, index N, class T, class K, class U, class OutIter>
OutIter intersect(const Policy& policy, const line<T, N, K>& l, const U& x, OutIter dst) {
    return intersect(line_scalar_policy{policy}, l, x, detail::scalar_to_point_adaptor{l, dst});
}

template <class T, class U, class OutIter>
OutIter intersect(const T& lhs, const U& rhs, OutIter dst) {
    return intersect(geometry_policy, lhs, rhs, dst);
}

} // namespace ac
