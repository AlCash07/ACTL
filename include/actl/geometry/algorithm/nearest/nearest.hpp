// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/traits.hpp>
#include <actl/std/utility.hpp>

namespace ac {

template <class T0, class T1>
    requires geometry::reverse_order<T, U>
auto nearest(Policy auto const& policy, T0 const& lhs, T1 const& rhs) {
    auto pair = nearest(policy, rhs, lhs);
    return std::pair{pair.second, pair.first};
}

template <class T, class... Ts, disable_int_if_policy<T> = 0>
auto nearest(T&& x, Ts&&... xs) {
    return nearest(geometry_policy, x, xs...);
}

} // namespace ac
