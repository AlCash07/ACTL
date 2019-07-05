/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/intersect/circle_circle.hpp>
#include <actl/geometry/algorithm/point/angle.hpp>
#include <actl/geometry/algorithm/point/norm.hpp>
#include <actl/geometry/algorithm/within/sphere.hpp>
#include <actl/iterator/output_type.hpp>

namespace ac {

template <class P = use_default>
struct tangents_point_circle_scalar : within_sphere<P> {};

template <class P = use_default>
struct tangents_point_circle_point : within_sphere<P> {
    intersect_circle_circle_point<P> intersect_policy;
};

template <class P, class T0, class T1, class OutIter>
inline auto tangents(const tangents_point_circle_scalar<P>& policy, const point<T0>& point,
                     const circle<T1>& circle, OutIter dst) {
    using O = geometry::scalar_t<output_type_t<OutIter>>;
    auto center_vector = circle.center - point;
    switch (within(policy, point, circle)) {
        case 0: {
            auto center_angle = angle(standard_angle<O, O>{}, center_vector);
            auto offset = math::atan2(static_cast<O>(circle.radius),
                                      norm(standard_norm<O, O>{}, center_vector));
            *dst++ = center_angle - offset;
            *dst++ = center_angle + offset;
            break;
        }
        case 1:
            *dst++ = angle(standard_angle<O, O>{}, perpendicular(center_vector));
    }
    return dst;
}

template <class P, class T0, class T1, class OutIter>
inline auto tangents(const tangents_point_circle_point<P>& policy, const point<T0>& point,
                     const circle<T1>& circle, OutIter dst) {
    using O = geometry::scalar_t<output_type_t<OutIter>>;
    auto center_vector = circle.center - point;
    switch (within(policy, point, circle)) {
        case 0: {
            auto dist = math::sqrt(norm(standard_norm<O, O>{}, center_vector) +
                                   sqr(static_cast<O>(circle.radius)));
            intersect(policy.intersect_policy, make_circle(point, dist), circle, dst);
            break;
        }
        case 1:
            *dst++ = point;
    }
    return dst;
}

namespace detail {

template <class T0, class T1, class OutIter>
inline auto tangents(const point<T0>& point, const circle<T1>& circle, OutIter dst, scalar_tag) {
    return tangents(tangents_point_circle_scalar<>{}, point, circle, dst);
}

template <class T0, class T1, class OutIter>
inline auto tangents(const point<T0>& point, const circle<T1>& circle, OutIter dst, point_tag) {
    return tangents(tangents_point_circle_point<>{}, point, circle, dst);
}

}  // namespace detail

template <class T0, class T1, class OutIter>
inline auto tangents(use_default, const point<T0>& point, const circle<T1>& circle, OutIter dst) {
    return detail::tangents(point, circle, dst, geometry::tag_t<output_type_t<OutIter>>{});
}

}  // namespace ac
