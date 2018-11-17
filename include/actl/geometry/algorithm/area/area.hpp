/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/traits/id.hpp>
#include <actl/geometry/traits/policy.hpp>

namespace ac {

template <class Policy, class T0, class T1, class = geometry::enable_if_swap_t<T0, T1>>
inline auto area(const Policy& policy, const T0& lhs, const T1& rhs) {
    if constexpr (geometry_traits<T0>::dimension == 2) {
        return -area(policy, rhs, lhs);
    } else {
        return area(policy, rhs, lhs);
    }
}

template <class T, class... Ts, class = geometry::disable_if_policy_t<T>>
inline auto area(const T& arg, const Ts&... args) {
    return area(use_default{}, arg, args...);
}

}  // namespace ac
