// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/orientation/point_line.hpp>
#include <actl/geometry/polygon.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/range/span.hpp>

namespace ac {

template <class P>
struct andrew_monotone_chain_policy
{
    andrew_monotone_chain_policy(P const& x) : policy{x} {}

    P const& policy;
};

/// Convex hull of a set of 2d points : O(N log N).
/// https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
/// Implementation reference:
/// https://github.com/stjepang/snippets/blob/master/convex_hull.cpp
template <class P, class T>
    requires(geometry_traits<T>::dimension == 2)
span<T> convex_hull(andrew_monotone_chain_policy<P> amcp, span<T> points)
{
    if (points.size() < 2)
        return points;
    auto& policy = amcp.policy;
    auto [a, b] = minmax_element(points, less(policy));
    auto comp = [l = make_line(*a, *b), &policy](auto const& p)
    {
        return !right_turn(policy, p, l);
    };
    index pivot = partition(points, comp) - points.begin();
    AC_ASSERT(2 <= pivot);
    sort(points.first(pivot), less(policy));
    index last = 1;
    auto pop = [&](auto const& p)
    {
        while (last != 0 &&
               !right_turn(policy, p, points[last], points[last - 1]))
            --last;
    };
    for (index i = 2, n = points.size(); i != n; ++i)
    {
        // TODO: somehow output the right-top point when this condition is met.
        if (i == pivot)
            sort(points.last(n - i), greater_functor(policy));
        pop(points[i]);
        points[++last] = points[i];
    }
    pop(points[0]);
    if (last == 1 && equal(policy, points[0], points[1]))
        --last;
    return points.first(last + 1);
}

template <class T>
span<T> convex_hull(span<T> points)
{
    return convex_hull(andrew_monotone_chain_policy{default_policy}, points);
}

} // namespace ac
