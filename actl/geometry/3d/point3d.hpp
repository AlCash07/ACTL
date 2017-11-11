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

template <int N, class T>
class point_xyz : public point_xy<N, T> {
public:
    using point_xy<N, T>::point_xy;

    constexpr T& z() { return (*this)[2]; }

    constexpr const T& z() const { return (*this)[2]; }
};

}  // namespace detail

/**
 * 3-dimensional point specialization.
 */
template <class T>
class point<3, T> : public detail::point_xyz<3, T> {
public:
    using detail::point_xyz<3, T>::point_xyz;
};

template <class T>
using point3d = point<3, T>;

template <class P = use_default, class T0, class T1>
inline constexpr point_type<3, T0, T1> cross(const point3d<T0>& lhs, const point3d<T1>& rhs) {
    point3d<deduce_type_t<P, T0>> lhs1 = lhs;
    return make_point(lhs1[1] * rhs[2] - lhs1[2] * rhs[1],
                      lhs1[2] * rhs[0] - lhs1[0] * rhs[2],
                      lhs1[0] * rhs[1] - lhs1[1] * rhs[0]);
}

}  // namespace ac
