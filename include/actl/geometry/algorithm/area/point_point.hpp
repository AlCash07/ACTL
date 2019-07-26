/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/sqrt.hpp>
#include <actl/geometry/2d/point2d.hpp>
#include <actl/geometry/3d/point3d.hpp>

namespace ac {

template <class P = use_default>
struct comparable_area_points : geometry::policy {};

template <class P = use_default, class F = use_default>
struct standard_area_points : geometry::policy {};

namespace detail {

template <class P, index N, class T0, class T1>
inline auto area2(const point<T0, N>& p0, const point<T1, N>& p1) {
    if constexpr (N == 3) {
        return dot<P>(cross<P>(p0, p1));
    } else {
        return dot<P>(p0) * dot<P>(p1) - sqr(dot<P>(p0, p1));
    }
}

}  // namespace detail

/**
 * Oriented area of parallelogram with side vectors being p0 and p1.
 * Positive if p1 <- p0 counter-clockwise.
 */
template <class P, class T0, class T1, class R = geometry::product_t<P, T0, T1>>
inline constexpr R area(comparable_area_points<P>, const point<T0>& p0, const point<T1>& p1) {
    return static_cast<R>(p0[0]) * p1[1] - static_cast<R>(p0[1]) * p1[0];
}

template <class P, class F, class T0, class T1>
inline constexpr auto area(standard_area_points<P, F>, const point<T0>& p0, const point<T1>& p1) {
    return area(comparable_area_points<P>{}, p0, p1);
}

// TODO: figure out the correct sign instead of returning the absolute value.
template <class P, index N, class T0, class T1>
inline auto area(comparable_area_points<P>, const point<T0, N>& p0, const point<T1, N>& p1) {
    return op::square_root{detail::area2<P>(p0, p1)};
}

template <class P, class F, index N, class T0, class T1>
inline auto area(standard_area_points<P, F>, const point<T0, N>& p0, const point<T1, N>& p1) {
    return static_cast<geometry::float_t<F, T0, T1>>(adl::sqrt(detail::area2<P>(p0, p1)));
}

template <index N, class T0, class T1>
inline auto area(use_default, const point<T0, N>& p0, const point<T1, N>& p1) {
    return area(standard_area_points{}, p0, p1);
}

}  // namespace ac
