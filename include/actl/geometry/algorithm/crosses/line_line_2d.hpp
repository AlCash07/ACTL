/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/algorithm/area/point_line.hpp>
#include <actl/geometry/detail/common_line.hpp>

namespace ac {

template <class AreaPolicy>
struct cross_line_line : AreaPolicy {};

template <class P = use_default>
using comparable_cross_line_line = cross_line_line<comparable_area_points<P>>;

template <class P = use_default, class S = use_default>
using standard_cross_line_line = cross_line_line<standard_area_points<P, S>>;

/**
 * This policy implies that lines are in general position (don't coincide but can be parallel).
 */
template <class AreaPolicy>
struct general_cross_line_line : AreaPolicy {};

template <class P = use_default>
using comparable_general_cross_line_line = general_cross_line_line<comparable_area_points<P>>;

template <class P = use_default, class S = use_default>
using standard_general_cross_line_line = general_cross_line_line<standard_area_points<P, S>>;

namespace detail {

template <class T0, class K0, class T1, class K1, class T2>
inline bool cross_test(const line<T0, 2, K0>& lhs, const line<T1, 2, K1>& rhs,
                       T2 tarea, T2 larea, T2 rarea) {
    if (tarea < T2{0}) {
        tarea = -tarea;
        larea = -larea;
        rarea = -rarea;
    }
    return line_test(lhs, larea, tarea) && line_test(rhs, rarea, tarea);
}

template <class R = use_default, class AP, class T0, class K0, class T1, class K1,
          class X = geometry::ratio_t<R, T0, T1>>
inline std::pair<bool, X> intersect_lines_general(const AP& policy, const line<T0, 2, K0>& lhs,
                                                  const line<T1, 2, K1>& rhs) {
    auto tarea = area(policy, rhs.slope, lhs.slope);
    if (tarea == 0) return {false, X{}};
    auto larea = area(policy, lhs.start, rhs);
    if (!cross_test(lhs, rhs, tarea, larea, area(policy, rhs.start, lhs))) return {false, X{}};
    return {true, static_cast<X>(larea) / tarea};
}

template <class R = use_default, class AP, class T0, class K0, class T1, class K1,
          class X = geometry::ratio_t<R, T0, T1>>
inline std::pair<bool, any_line<X, 2>> intersect_lines(const AP& policy, const line<T0, 2, K0>& lhs,
                                                       const line<T1, 2, K1>& rhs) {
    auto tarea = area(policy, rhs.slope, lhs.slope);
    auto larea = area(policy, lhs.start, rhs);
    if (tarea == 0) {
        if (larea != 0) return {false, any_line<X, 2>()};
        return common_line<X>(lhs, rhs);
    }
    if (!cross_test(lhs, rhs, tarea, larea, area(policy, rhs.start, lhs)))
        return {false, any_line<X, 2>()};
    return {true, any_line<X, 2>(lhs(static_cast<X>(larea) / tarea), point<X>(), true)};
}

}  // namespace detail

template <class AP, class T0, class K0, class T1, class K1>
inline bool crosses(const cross_line_line<AP> policy, const line<T0, 2, K0>& lhs,
                    const line<T1, 2, K1>& rhs) {
    return detail::intersect_lines(policy, lhs, rhs).first;
}

template <class AP, class T0, class K0, class T1, class K1>
inline bool crosses(const general_cross_line_line<AP> policy, const line<T0, 2, K0>& lhs,
                    const line<T1, 2, K1>& rhs) {
    return detail::intersect_lines_general(policy, lhs, rhs).first;
}

template <class T0, class K0, class T1, class K1>
inline bool crosses(use_default, const line<T0, 2, K0>& lhs, const line<T1, 2, K1>& rhs) {
    return crosses(comparable_cross_line_line<>(), lhs, rhs);
}

}  // namespace ac
