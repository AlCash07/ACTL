/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/polygon/winding_number.hpp>
#include <actl/geometry/algorithm/within/within.hpp>

namespace ac {

/**
 * O(N).
 */
template <class Policy, class T, class U>
inline int within(const Policy& policy, const point<T>& p, const simple_polygon<U>& poly) {
    switch (winding_number(policy, p, poly)) {
        case 0: return 0;
        case std::numeric_limits<int>::max(): return 1;
        default: return 2;
    }
}

}  // namespace ac
