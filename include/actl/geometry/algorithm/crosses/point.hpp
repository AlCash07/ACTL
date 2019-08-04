/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/crosses/crosses.hpp>
#include <actl/geometry/algorithm/within/within.hpp>
#include <actl/geometry/point.hpp>

namespace ac {

template <class Policy, index N, class T, class U>
inline bool crosses(const Policy& policy, const point<T, N>& lhs, const U& rhs) {
    return within(policy, lhs, rhs) == within::border;
}

}  // namespace ac
