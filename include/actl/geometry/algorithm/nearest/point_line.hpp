// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/nearest/nearest.hpp>
#include <actl/geometry/algorithm/project/point_line.hpp>

namespace ac {

template <class Policy, index N, class T0, class T1, class K>
auto nearest(
    Policy const& policy, point<T0, N> const& p, line<T1, N, K> const& l)
{
    using Pair =
        std::pair<point<T0, N> const&, decltype(project(policy, p, l))>;
    if (begin(l.kind()) != endpoint::free &&
        !less(policy, 0, dot(policy, p - l.begin, l.vector)))
        return Pair{p, l.begin};
    if (end(l.kind()) != endpoint::free &&
        !less(policy, dot(policy, p - l.end(), l.vector), 0))
        return Pair{p, l.end()};
    return Pair{p, project(policy, p, l)};
}

} // namespace ac
