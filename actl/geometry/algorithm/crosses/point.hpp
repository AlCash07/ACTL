/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/within/within.hpp>
#include <actl/geometry/point.hpp>

namespace ac {

template <class WithinPolicy>
struct crosses_point : WithinPolicy {};

template <class WP, int N, class T0, class T1>
inline bool crosses(const crosses_point<WP>& policy, const point<T0, N>& lhs, const T1& rhs) {
    return within(policy, lhs, rhs) == 0;
}

template <int N, class T0, class T1>
inline bool crosses(use_default, const point<T0, N>& lhs, const T1& rhs) {
    return within(use_default(), lhs, rhs) == 0;
}

}  // namespace ac
