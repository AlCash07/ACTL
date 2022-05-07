// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/project/project.hpp>
#include <actl/geometry/point.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1>
auto& project(
    Policy const& policy, point<T0, N> const&, point<T1, N> const& dst)
{
    return dst;
}

} // namespace ac
