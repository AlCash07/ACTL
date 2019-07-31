/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/traits.hpp>
#include <actl/std/utility.hpp>

namespace ac {

template <class Policy, class T0, class T1, geometry::enable_int_if_swap<T0, T1> = 0>
inline auto nearest(const Policy& policy, const T0& lhs, const T1& rhs) {
    auto pair = nearest(policy, rhs, lhs);
    return std::pair{pair.second, pair.first};
}

template <class T, class... Ts, disable_int_if_policy<T> = 0>
inline auto nearest(T&& x, Ts&&... xs) {
    return nearest(geometry_policy, x, xs...);
}

}  // namespace ac
