// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/geometry/traits.hpp>

namespace ac {

template <class Policy, class T, class U, geometry::enable_int_if_swap<T, U> = 0>
auto area(const Policy& policy, const T& lhs, const U& rhs) {
    if constexpr (geometry_traits<T>::dimension == 2) {
        return -area(policy, rhs, lhs);
    } else {
        return area(policy, rhs, lhs);
    }
}

template <class T, class... Ts, disable_int_if_policy<T> = 0>
auto area(const T& x, const Ts&... xs) {
    return area(geometry_policy, x, xs...);
}

}  // namespace ac
