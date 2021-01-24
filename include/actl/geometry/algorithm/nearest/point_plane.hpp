// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/nearest/nearest.hpp>
#include <actl/geometry/algorithm/project/point_plane.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1>
auto nearest(const Policy& policy, const point<T0, N>& p, const plane<T1, N>& pl) {
    return std::pair{p, project(policy, p, pl)};
}

} // namespace ac
