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

template<typename L, typename R, typename OutIter>
    requires geometry::reverse_order<L, R>
OutIter intersect(
    Policy auto const& policy, L const& l, R const& r, OutIter output
) {
    return intersect(policy, r, l, output);
}

template<index N, typename T, typename K, typename R, typename OutIter>
OutIter intersect(
    Policy auto const& policy,
    line<T, N, K> const& l,
    R const& r,
    OutIter output
) {
    return intersect(
        line_scalar_policy{policy},
        l,
        r,
        detail::scalar_to_point_adaptor{l, output}
    );
}

template<typename L, typename R, typename OutIter>
OutIter intersect(L const& l, R const& r, OutIter output) {
    return intersect(geometry_policy, l, r, output);
}

} // namespace ac
