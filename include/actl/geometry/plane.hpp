/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/3d/point3d.hpp>

namespace ac {

/**
 * Plane with equation in the form: dot(normal, x) = d.
 * In 2D represents a line.
 */
template <class T, int N = 3>
struct plane {
    point<T, N> normal;  // normal vector
    T           d;       // distance from origin to the plane times the normal norm

    explicit constexpr plane() = default;

    template <class T1, class T2>
    explicit constexpr plane(const point<T1, N>& normal, const T2& d)
        : normal(normal), d{static_cast<T>(d)} {}

    template <class T1, class T2>
    explicit constexpr plane(const point<T1, N>& normal, const point<T2, N>& point)
        : plane(normal, dot(normal, point)) {}

    template <class T1>
    explicit constexpr plane(const plane<T1, N>& other) { (*this) = other; }

    explicit constexpr operator bool() const { return normal; }

    void swap(plane& other) {
        using std::swap;
        swap(normal, other.normal);
        swap(d, other.d);
    }

    template <class T1>
    constexpr plane& operator = (const plane<T1, N>& other) {
        normal = other.normal;
        d = static_cast<T>(other.d);
        return *this;
    }

    // Oriented distance from @p point to the plane times the normal norm.
    template <class P = use_default, class T1>
    constexpr auto operator()(const point<T1, N>& point) const {
        return dot<P>(normal, point) - d;
    };
};

template <int N, class T>
struct geometry_traits<plane<T, N>> : geometry_traits_base<plane_tag, point<T, N>> {};

template <int N, class... Ts>
using plane_type = plane<geometry::scalar_t<Ts...>, N>;

template <int N, class T0, class T1>
inline constexpr auto make_plane(const point<T0, N>& normal, const T1& d) {
    return plane_type<N, T0, T1>(normal, d);
}

template <int N, class T0, class T1>
inline constexpr auto make_plane(const point<T0, N>& normal, const point<T1, N>& point) {
    return plane_type<N, T0, T1>(normal, point);
}

template <class T0, class T1>
inline constexpr auto make_plane2d(const point<T0>& a, const point<T1>& b) {
    return plane_type<2, T0, T1>(perpendicular(b - a), a);
}

template <class T0, class T1, class T2>
inline constexpr auto make_plane3d(const point3d<T0>& a, const point3d<T1>& b,
                                   const point3d<T2>& c) {
    return plane_type<3, T0, T1, T2>(cross(b - a, c - a), a);
}

template <int N, class T>
inline void swap(plane<T, N>& lhs, plane<T, N>& rhs) { lhs.swap(rhs); }

template <class Device, int N, class T>
inline bool read(Device& in, plane<T, N>& arg) {
    return read(in, arg.normal, arg.d);
}

template <class Device, int N, class T>
inline int write(Device& out, const plane<T, N>& arg) {
    return write(out, arg.normal, arg.d);
}

}  // namespace ac
