// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/traits.hpp>
#include <actl/std/utility.hpp>

namespace ac {

template<typename L, typename R>
    requires geometry::reverse_order<L, R>
auto nearest(Policy auto const& policy, L const& l, R const& r) {
    auto pair = nearest(policy, r, l);
    return std::pair{pair.second, pair.first};
}

template<typename T, typename... Ts, disable_int_if_policy<T> = 0>
auto nearest(T&& x, Ts&&... xs) {
    return nearest(geometry_policy, x, xs...);
}

} // namespace ac
