/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/polygon/winding_number.hpp>

namespace ac {

template <class WindingPolicy = winding_number_policy<>>
struct within_simple_polygon : WindingPolicy {};

/**
 * O(N).
 */
template <class CP, class T0, class T1>
inline int within(const within_simple_polygon<CP>& policy, const point<T0>& point,
                  const simple_polygon<T1>& polygon) {
    switch (winding_number(policy, point, polygon)) {
        case std::numeric_limits<int>::max(): return 0;
        case 0: return 1;
        default: return -1;
    }
}

template <class T0, class T1>
inline int within(use_default, const point<T0>& point, const simple_polygon<T1>& polygon) {
    return within(within_simple_polygon<>(), point, polygon);
}

}  // namespace ac
