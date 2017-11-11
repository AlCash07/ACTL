/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functions.hpp>
#include <actl/geometry/point.hpp>

namespace ac {

namespace detail {

template <int N, class T>
class point_xy : public point_base<N, T> {
public:
    using point_base<N, T>::point_base;

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
class point<2, T> : public detail::point_xy<2, T> {
public:
    using detail::point_xy<2, T>::point_xy;
};

template <class T>
using point2d = point<2, T>;

template <class T0, class T1>
inline constexpr bool y_compare(const point2d<T0>& lhs, const point2d<T1>& rhs) {
    return lhs[1] < rhs[1] || (lhs[1] == rhs[1] && lhs[0] < rhs[0]);
}

/**
 * Point @p src rotated by pi/2 counter-clockwise.
 */
template <class T>
inline constexpr point2d<T> perpendicular(const point2d<T>& src) {
    return make_point(-src[1], src[0]);
}

}  // namespace ac
