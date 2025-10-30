// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/traits.hpp>

namespace ac {

template<typename L, typename R>
    requires geometry::reverse_order<L, R>
auto area(Policy auto const& policy, L const& l, R const& r) {
    if constexpr (geometry_traits<L>::dimension == 2) {
        return -area(policy, r, l);
    } else {
        return area(policy, r, l);
    }
}

template<typename T, typename... Ts, disable_int_if_policy<T> = 0>
auto area(T const& x, Ts const&... xs) {
    return area(geometry_policy, x, xs...);
}

} // namespace ac
