// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

template <index N, class T, class U>
decltype(auto) project(const point<T, N>& src, const U& dst)
{
    return project(geometry_policy, src, dst);
}

} // namespace ac
