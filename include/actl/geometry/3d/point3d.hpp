/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/2d/point2d.hpp>

namespace ac {

namespace detail {

template <class T, index N>
class point_xyz : public point_xy<T, N> {
public:
    using point_xy<T, N>::point_xy;

    constexpr T& z() { return (*this)[2]; }

    constexpr const T& z() const { return (*this)[2]; }
};

}  // namespace detail

/**
 * 3-dimensional point specialization.
 */
template <class T>
class point<T, 3> : public detail::point_xyz<T, 3> {
public:
    using detail::point_xyz<T, 3>::point_xyz;
};

template <class T>
using point3d = point<T, 3>;

template <class Policy, class T0, class T1>
inline constexpr auto cross(const Policy& policy, const point3d<T0>& lhs, const point3d<T1>& rhs) {
    return point{product(policy, lhs[1], rhs[2]) - product(policy, lhs[2], rhs[1]),
                 product(policy, lhs[2], rhs[0]) - product(policy, lhs[0], rhs[2]),
                 product(policy, lhs[0], rhs[1]) - product(policy, lhs[1], rhs[0])};
}

template <class T0, class T1>
inline constexpr auto cross(const point3d<T0>& lhs, const point3d<T1>& rhs) {
    return cross(default_policy, lhs, rhs);
}

}  // namespace ac
