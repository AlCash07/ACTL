// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

template<index N, typename T, typename U>
auto reflect(point<T, N> const& p, U const& across) {
    return reflect(geometry_policy, p, across);
}

} // namespace ac
