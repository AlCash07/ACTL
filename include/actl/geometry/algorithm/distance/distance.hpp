// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/traits.hpp>

namespace ac {

template<class T, class U>
    requires geometry::reverse_order<T, U>
auto distance(Policy auto const& policy, T const& lhs, U const& rhs) {
    return distance(policy, rhs, lhs);
}

template<class T, class U>
auto distance(T const& lhs, U const& rhs) {
    return distance(geometry_policy, rhs, lhs);
}

} // namespace ac
