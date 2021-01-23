// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

template <index N, class T, class U>
auto reflect(const point<T, N>& src, const U& dst) {
    return reflect(geometry_policy, src, dst);
}

}  // namespace ac
