// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/polygon/extreme_vertex.hpp>
#include <actl/geometry/algorithm/tangents/tangents.hpp>
#include <actl/std/utility.hpp>

namespace ac {

/// Returns pair of iterators to polygon vertices tangent w.r.t. the given
/// point. Point must be outside the polygon.
template <class T, class U, class OutIter>
OutIter tangents(
    Policy auto const& policy,
    point<T> const& p,
    convex_polygon<U> const& poly,
    OutIter dst)
{
    *dst++ =
        extreme_vertex(policy, poly, [&p](auto const& x) { return x - p; });
    *dst++ =
        extreme_vertex(policy, poly, [&p](auto const& x) { return p - x; });
    return dst;
}

} // namespace ac
