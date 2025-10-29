// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/traits.hpp>

namespace ac {

template<typename T, typename U>
    requires geometry::reverse_order<T, U>
auto area(Policy auto const& policy, T const& lhs, U const& rhs) {
    if constexpr (geometry_traits<T>::dimension == 2) {
        return -area(policy, rhs, lhs);
    } else {
        return area(policy, rhs, lhs);
    }
}

template<typename T, typename... Ts, disable_int_if_policy<T> = 0>
auto area(T const& x, Ts const&... xs) {
    return area(geometry_policy, x, xs...);
}

} // namespace ac
