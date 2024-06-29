// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/polygon/winding_number.hpp>
#include <actl/geometry/algorithm/within/within.hpp>

namespace ac {

/// O(N).
template <class T, class U>
enum within within(
    Policy auto const& policy, point<T> const& p, simple_polygon<U> const& poly
) {
    switch (winding_number(policy, p, poly)) {
        case 0:
            return within::outside;
        case std::numeric_limits<int>::max():
            return within::border;
        default:
            return within::inside;
    }
}

} // namespace ac
