/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/sqrt.hpp>
#include <actl/geometry/point.hpp>

namespace ac {

template <class P = use_default>
struct comparable_norm : geometry::policy {};

template <class P = use_default, class F = use_default>
struct standard_norm : geometry::policy {};

template <class Policy, index N, class T>
inline auto norm(const Policy& policy, const point<T, N>& p) {
    return sqrt(policy, dot(policy, p));
}

template <index N, class T>
inline auto norm(const point<T, N>& p) {
    return norm(geometry_policy, p);
}

}  // namespace ac
