/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/point.hpp>
#include <actl/numeric/functions.hpp>

namespace ac {

namespace detail {

template <class T, index N>
class point_xy : public point_base<T, N> {
public:
    using point_base<T, N>::point_base;

    constexpr T& x() { return (*this)[0]; }
    constexpr T& y() { return (*this)[1]; }

    constexpr const T& x() const { return (*this)[0]; }
    constexpr const T& y() const { return (*this)[1]; }
};

}  // namespace detail

/**
 * 2-dimensional point specialization.
 */
template <class T>
class point<T, 2> : public detail::point_xy<T, 2> {
public:
    using detail::point_xy<T, 2>::point_xy;
};

template <class T>
using point2d = point<T, 2>;

template <class T0, class T1>
inline constexpr bool y_compare(const point<T0>& lhs, const point<T1>& rhs) {
    return lhs[1] < rhs[1] || (lhs[1] == rhs[1] && lhs[0] < rhs[0]);
}

/**
 * Point @p src rotated by pi/2 counter-clockwise.
 */
template <class T>
inline constexpr point<T> perpendicular(const point<T>& src) {
    return point{-src[1], src[0]};
}

}  // namespace ac
