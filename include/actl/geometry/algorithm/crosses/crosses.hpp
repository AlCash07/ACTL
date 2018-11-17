/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/traits/id.hpp>
#include <actl/util/use_default.hpp>

namespace ac {

template <class Policy, class T0, class T1, class = geometry::enable_if_swap_t<T0, T1>>
inline bool crosses(const Policy& policy, const T0& lhs, const T1& rhs) {
    return crosses(policy, rhs, lhs);
}

template <class T0, class T1>
inline bool crosses(const T0& lhs, const T1& rhs) {
    return crosses(use_default{}, rhs, lhs);
}

}  // namespace ac
