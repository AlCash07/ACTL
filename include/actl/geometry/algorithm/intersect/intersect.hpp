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

template <class T, class U, class OutIter>
    requires geometry::reverse_order<T, U>
OutIter intersect(
    Policy auto const& policy, T const& lhs, U const& rhs, OutIter dst
) {
    return intersect(policy, rhs, lhs, dst);
}

template <index N, class T, class K, class U, class OutIter>
OutIter intersect(
    Policy auto const& policy, line<T, N, K> const& l, U const& x, OutIter dst
) {
    return intersect(
        line_scalar_policy{policy},
        l,
        x,
        detail::scalar_to_point_adaptor{l, dst}
    );
}

template <class T, class U, class OutIter>
OutIter intersect(T const& lhs, U const& rhs, OutIter dst) {
    return intersect(geometry_policy, lhs, rhs, dst);
}

} // namespace ac
