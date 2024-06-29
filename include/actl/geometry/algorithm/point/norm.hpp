// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/point.hpp>
#include <actl/numeric/math.hpp>

namespace ac {

template<index N, class T>
auto norm(Policy auto const& policy, point<T, N> const& p) {
    return sqrt(policy, dot(policy, p));
}

template<index N, class T>
auto norm(point<T, N> const& p) {
    return norm(geometry_policy, p);
}

} // namespace ac
