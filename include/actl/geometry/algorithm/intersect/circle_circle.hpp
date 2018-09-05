/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/2d/circle.hpp>
#include <actl/geometry/algorithm/point/angle.hpp>
#include <actl/geometry/traits/aliases.hpp>
#include <actl/geometry/traits/policy.hpp>
#include <actl/math.hpp>
#include <actl/traits/output_type.hpp>

namespace ac {

struct intersect_circle_circle_scalar : geometry::policy {};

template <class P = use_default>
struct intersect_circle_circle_point : geometry::policy {};

// TODO: implement using cosine theorem.
template <class T0, class T1, class OutputIterator>
inline auto intersect(intersect_circle_circle_scalar, const circle<T0>& lhs, const circle<T1>& rhs,
                      OutputIterator dst) {
    using O = geometry::scalar_t<output_type_t<OutputIterator>>;
    auto centers_vector = rhs.center - lhs.center;
    auto centers_angle  = angle(standard_angle<O, O>(), centers_vector);
    return dst;
}

template <class P, class T0, class T1, class OutputIterator>
inline auto intersect(intersect_circle_circle_point<P>, const circle<T0>& lhs,
                      const circle<T1>& rhs, OutputIterator dst) {
    using X = geometry::product_t<P, T0, T1>;
    using O = geometry::scalar_t<output_type_t<OutputIterator>>;
    auto centers_vector = rhs.center - lhs.center;
    auto centers_dist   = abs<X>(centers_vector);
    X lradius = static_cast<X>(lhs.radius);
    X rradius = static_cast<X>(rhs.radius);
    auto sgn0 = sgn(centers_dist, sqr(lradius - rradius));
    auto sgn1 = sgn(sqr(lradius + rradius), centers_dist);
    if (sgn0 < 0 || sgn1 < 0) return dst;
    auto a = static_cast<O>(sqr(lradius) - sqr(rradius)) / centers_dist;
    point<O> projection = lhs.center + centers_vector * (a + 1) / 2;
    if (sgn0 == 0 || sgn1 == 0) {
        *dst++ = projection;
    } else {
        auto b = static_cast<O>(sqr(lradius) + sqr(rradius)) / centers_dist;
        auto offset = math::sqrt(2 * b - sqr(a) - 1) / 2;
        *dst++ = projection - perpendicular(centers_vector) * offset;
        *dst++ = projection + perpendicular(centers_vector) * offset;
    }
    return dst;
}

namespace detail {

template <class T0, class T1, class It>
inline auto intersect(const circle<T0>& lhs, const circle<T1>& rhs, It dst, scalar_tag) {
    return ac::intersect(intersect_circle_circle_scalar(), lhs, rhs, dst);
}

template <class T0, class T1, class It>
inline auto intersect(const circle<T0>& lhs, const circle<T1>& rhs, It dst, point_tag) {
    return ac::intersect(intersect_circle_circle_point<>(), lhs, rhs, dst);
}

}  // namespace detail

template <class T0, class T1, class OutputIterator>
inline auto intersect(use_default, const circle<T0>& lhs, const circle<T1>& rhs,
                      OutputIterator dst) {
    return detail::intersect(lhs, rhs, dst, geometry::tag_t<output_type_t<OutputIterator>>());
}

}  // namespace ac