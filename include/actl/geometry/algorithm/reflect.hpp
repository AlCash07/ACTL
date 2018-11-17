/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/project/project.hpp>

namespace ac {

template <class Policy, int N, class T0, class T1>
inline auto reflect(const Policy& policy, const point<T0, N>& src, const T1& dst) {
    return 2 * project(policy, src, dst) - src;
}

template <int N, class T0, class T1>
inline auto reflect(const point<T0, N>& src, const T1& dst) {
    return reflect(use_default{}, src, dst);
}

}  // namespace ac
