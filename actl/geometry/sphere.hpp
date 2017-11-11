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
template <int N, class T>
struct sphere {
    point<N, T> center;
    T           radius;

    explicit constexpr sphere() = default;

    template <class T1, class T2>
    explicit constexpr sphere(const point<N, T1>& center, const T2& radius)
        : center(center), radius{static_cast<T>(radius)} {}

    template <class T1>
    explicit constexpr sphere(const sphere<N, T1>& other) { (*this) = other; }

    explicit constexpr operator bool () const { return radius > T{0}; }

    void swap(sphere& other) {
        using std::swap;
        swap(center, other.center);
        swap(radius, other.radius);
    }

    template <class T1>
    constexpr sphere& operator = (const sphere<N, T1>& other) {
        center = other.center;
        radius = static_cast<T>(other.radius);
        return *this;
    }
};

template <int N, class T>
struct geometry_traits<sphere<N, T>> : geometry_traits_base<sphere_tag, point<N, T>> {};

template <int N, class T1, class T2>
inline constexpr auto make_sphere(const point<N, T1>& center, const T2& radius) {
    return sphere<N, geometry::scalar_t<T1, T2>>(center, radius);
}

template <int N, class T>
inline void swap(sphere<N, T>& lhs, sphere<N, T>& rhs) { lhs.swap(rhs); }

template <class Device, int N, class T>
inline bool read(Device& in, sphere<N, T>& arg) {
    return read(in, arg.center, arg.radius);
}

template <class Device, int N, class T>
inline int write(Device& out, const sphere<N, T>& arg) {
    return write(out, arg.center, arg.radius);
}

}  // namespace ac
