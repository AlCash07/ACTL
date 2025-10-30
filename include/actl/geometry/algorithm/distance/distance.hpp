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
auto distance(Policy auto const& policy, L const& l, R const& r) {
    return distance(policy, r, l);
}

template<typename L, typename R>
auto distance(L const& l, R const& r) {
    return distance(geometry_policy, r, l);
}

} // namespace ac
