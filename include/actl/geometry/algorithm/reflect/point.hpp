// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/project/project.hpp>

namespace ac {

template <class Policy, index N, class T, class U>
auto reflect(const Policy& policy, const point<T, N>& src, const U& dst)
{
    return product(policy, 2, project(policy, src, dst)) - src;
}

template <index N, class T, class U>
auto reflect(const point<T, N>& src, const U& dst)
{
    return reflect(geometry_policy, src, dst);
}

} // namespace ac
