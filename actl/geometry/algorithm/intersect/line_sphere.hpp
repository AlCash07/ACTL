/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/area/point_line.hpp>
#include <actl/geometry/detail/line_test.hpp>
#include <actl/geometry/detail/scalar_to_point_adaptor.hpp>
#include <actl/geometry/sphere.hpp>

namespace ac {

template <class P = use_default, class AreaPolicy = comparable_area_points<P>>
struct intersect_line_sphere : AreaPolicy {};

namespace detail {

template <class P, class AP, int N, class T0, class K, class T1, class It>
inline auto intersect(const intersect_line_sphere<P, AP>& policy, const line<N, T0, K>& line,
                      const sphere<N, T1>& sphere, It dst) {
    using O = geometry::scalar_t<output_type_t<It>>;
    auto slope_abs = abs<P>(line.slope);
    auto delta = slope_abs * sqr(static_cast<geometry::product_t<P, T0, T1>>(sphere.radius)) -
                 sqr(area(policy, sphere.center, line));
    int delta_sgn = sgn(delta);
    if (delta_sgn < 0) return dst;
    O projection = static_cast<O>(dot<P>(sphere.center - line.start, line.slope));
    auto output = [&line, &dst, slope_abs1 = static_cast<O>(slope_abs)](const O& num) {
        if (detail::line_test(line, num, slope_abs1)) *dst++ = num / slope_abs1;
    };
    if (delta_sgn == 0) {
        output(projection);
    } else {
        O offset = math::sqrt(static_cast<O>(delta));
        output(projection - offset);
        output(projection + offset);
    }
    return dst;
}

}  // namespace detail

template <class P, int N, class T0, class K, class T1, class OutputIterator>
inline OutputIterator intersect(const intersect_line_sphere<P>& policy, const line<N, T0, K>& line,
                                const sphere<N, T1>& sphere, OutputIterator dst) {
    return detail::intersect(policy, line, sphere, detail::adapt_iterator(line, dst));
}

template <int N, class T0, class K, class T1, class OutputIterator>
inline auto intersect(use_default, const line<N, T0, K>& line, const sphere<N, T1>& sphere,
                      OutputIterator dst) {
    return intersect(intersect_line_sphere<>(), line, sphere, dst);
}

}  // namespace ac
