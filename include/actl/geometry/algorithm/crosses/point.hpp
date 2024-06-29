// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/crosses/crosses.hpp>
#include <actl/geometry/algorithm/within/within.hpp>
#include <actl/geometry/point.hpp>

namespace ac {

template<index N, class T, class U>
bool crosses(Policy auto const& policy, point<T, N> const& lhs, U const& rhs) {
    return within(policy, lhs, rhs) == within::border;
}

} // namespace ac
