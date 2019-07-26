/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/traits.hpp>

namespace ac {

template <class Policy, class T0, class T1, geometry::enable_int_if_swap<T0, T1> = 0>
inline auto area(Policy&& policy, const T0& lhs, const T1& rhs) {
    if constexpr (geometry_traits<T0>::dimension == 2) {
        return -area(policy, rhs, lhs);
    } else {
        return area(policy, rhs, lhs);
    }
}

template <class T, class... Ts, geometry::disable_int_if_policy<T> = 0>
inline auto area(const T& arg, const Ts&... args) {
    return area(use_default{}, arg, args...);
}

}  // namespace ac
