/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

/**
 * N-dimensional sphere.
 */
template <class T, int N = 3>
struct sphere {
    point<T, N> center;
    T           radius;

    explicit constexpr sphere() = default;

    template <class T1, class T2>
    explicit constexpr sphere(const point<T1, N>& center, const T2& radius)
        : center(center), radius{static_cast<T>(radius)} {}

    template <class T1>
    explicit constexpr sphere(const sphere<T1, N>& rhs) { (*this) = rhs; }

    explicit constexpr operator bool() const { return radius > T{0}; }

    void swap(sphere& rhs) {
        using std::swap;
        swap(center, rhs.center);
        swap(radius, rhs.radius);
    }

    template <class T1>
    constexpr sphere& operator = (const sphere<T1, N>& rhs) {
        center = rhs.center;
        radius = static_cast<T>(rhs.radius);
        return *this;
    }
};

template <int N, class T>
struct geometry_traits<sphere<T, N>> : geometry_traits_base<sphere_tag, point<T, N>> {};

template <int N, class T0, class T1>
inline constexpr auto make_sphere(const point<T0, N>& center, const T1& radius) {
    return sphere<geometry::scalar_t<T0, T1>, N>(center, radius);
}

template <int N, class T>
inline void swap(sphere<T, N>& lhs, sphere<T, N>& rhs) { lhs.swap(rhs); }

template <class Device, int N, class T>
inline bool read(Device& in, sphere<T, N>& arg) {
    return read(in, arg.center, arg.radius);
}

template <class Device, int N, class T>
inline int write(Device& out, const sphere<T, N>& arg) {
    return write(out, arg.center, arg.radius);
}

}  // namespace ac
