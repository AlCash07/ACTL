/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/2d/point2d.hpp>
#include <actl/geometry/3d/point3d.hpp>
#include <actl/geometry/traits/aliases.hpp>
#include <actl/geometry/traits/policy.hpp>

namespace ac {

template <class P = use_default>
struct comparable_area_points : geometry::policy {};

template <class P = use_default, class S = use_default>
struct standard_area_points : geometry::policy {};

/**
 * Oriented area of parallelogram with side vectors being lhs and rhs.
 * Positive if rhs <- lhs counter-clockwise.
 */
template <class P, class T0, class T1, class X = geometry::product_t<P, T0, T1>>
inline constexpr X area(comparable_area_points<P>, const point<T0>& lhs, const point<T1>& rhs) {
    return static_cast<X>(lhs[0]) * rhs[1] - static_cast<X>(lhs[1]) * rhs[0];
}

template <class P, class S, class T0, class T1>
inline constexpr auto area(standard_area_points<P, S>, const point<T0>& lhs, const point<T1>& rhs) {
    return area(comparable_area_points<P>(), lhs, rhs);
}

// TODO: figure out the correct sign instead of returning the absolute value.
template <class P, class T0, class T1>
inline auto area(comparable_area_points<P>, const point3d<T0>& lhs, const point3d<T1>& rhs) {
    return deferred_sqrt(abs<P>(cross<P>(lhs, rhs)));
}

// TODO: figure out the correct sign instead of returning the absolute value.
template <class P, int N, class T0, class T1>
inline auto area(comparable_area_points<P>, const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return deferred_sqrt(abs<P>(lhs) * abs<P>(rhs) - sqr(dot<P>(lhs, rhs)));
}

template <class P, class S, int N, class T0, class T1>
inline auto area(standard_area_points<P, S>, const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return static_cast<geometry::sqrt_t<S, T0, T1>>(area(comparable_area_points<P>(), lhs, rhs));
}

template <int N, class T0, class T1>
inline auto area(use_default, const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return area(standard_area_points<>(), lhs, rhs);
}

}  // namespace ac
