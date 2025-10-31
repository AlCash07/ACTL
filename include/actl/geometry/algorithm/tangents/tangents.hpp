// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/traits.hpp>

namespace ac {

template<typename L, typename R, typename OutIter>
    requires geometry::reverse_order<L, R>
OutIter tangents(
    Policy auto const& policy, L const& l, R const& r, OutIter output
) {
    return tangents(policy, r, l, output);
}

template<typename L, typename R, typename OutIter>
OutIter tangents(L const& l, R const& r, OutIter output) {
    return tangents(geometry_policy, l, r, output);
}

} // namespace ac
