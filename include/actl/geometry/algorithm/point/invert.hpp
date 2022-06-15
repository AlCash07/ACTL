// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/point.hpp>
#include <actl/geometry/sphere.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1>
auto invert(Policy const& policy, point<T0, N> const& p, T1 const& radius)
{
    AC_ASSERT(!degenerate(policy, p));
    auto t = ratio(policy, sqr(policy, radius), dot(policy, p));
    return product(policy, p, t);
}

template <class Policy, index N, class T0, class T1>
auto invert(Policy const& policy, point<T0, N> const& p, sphere<T1, N> const& s)
{
    return s.center + invert(policy, p - s.center, s.radius);
}

template <index N, class T, class U>
auto invert(point<T, N> const& p, U const& x)
{
    return invert(geometry_policy, p, x);
}

} // namespace ac
