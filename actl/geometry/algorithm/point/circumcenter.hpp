/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/2d/point2d.hpp>
#include <actl/geometry/algorithm/intersect/line_line_2d.hpp>

namespace ac {

template <class R = use_default, class IntersectPolicy = intersect_line_line<>>
struct circumcenter_policy : IntersectPolicy {};

template <class R, class IP, class T0, class T1, class T2>
inline auto circumcenter(const circumcenter_policy<R, IP>& policy, const point2d<T0>& a,
                         const point2d<T1>& b, const point2d<T2>& c) {
    point2d<geometry::ratio_t<R, T0, T1, T2>> res;
    intersect(policy, make_line(a + b, perpendicular(a - b), true),
              make_line(a + c, perpendicular(a - c), true), &res);
    return res / 2;
}

template <class T0, class T1, class T2>
inline auto circumcenter(const point2d<T0>& a, const point2d<T1>& b, const point2d<T2>& c) {
    return circumcenter(circumcenter_policy<>(), a, b, c);
}

}  // namespace ac
