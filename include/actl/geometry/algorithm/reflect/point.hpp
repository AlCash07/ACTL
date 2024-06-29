// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/project/project.hpp>

namespace ac {

template <index N, class T, class U>
auto reflect(Policy auto const& policy, point<T, N> const& src, U const& dst) {
    return product(policy, 2, project(policy, src, dst)) - src;
}

template <index N, class T, class U>
auto reflect(point<T, N> const& src, U const& dst) {
    return reflect(geometry_policy, src, dst);
}

} // namespace ac
