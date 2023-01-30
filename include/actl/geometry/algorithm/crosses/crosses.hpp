// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/traits.hpp>

namespace ac {

template <class T, class U>
    requires geometry::reverse_order<T, U> bool
crosses(Policy auto const& policy, T const& lhs, U const& rhs)
{
    return crosses(policy, rhs, lhs);
}

template <class T, class U>
bool crosses(T const& lhs, U const& rhs)
{
    return crosses(geometry_policy, rhs, lhs);
}

} // namespace ac
