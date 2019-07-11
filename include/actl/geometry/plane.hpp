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
template <class T, index N = 3>
class plane {
public:
    point<T, N> normal;  // normal vector
    T d;                 // distance from origin to the plane times the norm of normal

    constexpr plane() = default;

    template <class T1, class T2>
    explicit constexpr plane(const point<T1, N>& normal, const T2& d)
        : normal{normal}, d{static_cast<T>(d)} {}

    template <class T1, class T2>
    explicit constexpr plane(const point<T1, N>& normal, const point<T2, N>& point)
        : plane{normal, dot<T>(normal, point)} {}

    template <class T1>
    explicit constexpr plane(const plane<T1, N>& rhs) {
        normal = rhs.normal;
        d = static_cast<T>(rhs.d);
    }

    explicit constexpr operator bool() const { return normal; }

    void swap(plane& rhs) {
        using std::swap;
        swap(normal, rhs.normal);
        swap(d, rhs.d);
    }

    // Oriented distance from @p point to the plane times the norm of normal.
    template <class P = use_default, class T1>
    constexpr auto operator()(const point<T1, N>& point) const {
        return dot<P>(normal, point) - d;
    };

private:
    INTROSPECT(normal, d)
};

template <index N, class T0, class T1>
plane(const point<T0, N>&, const T1&) -> plane<geometry::scalar_t<T0, T1>, N>;

template <index N, class T0, class T1>
plane(const point<T0, N>&, const point<T1, N>&) -> plane<geometry::scalar_t<T0, T1>, N>;

template <index N, class T>
struct geometry_traits<plane<T, N>> : geometry_traits_base<plane_tag, point<T, N>> {};

template <index N, class... Ts>
using plane_t = plane<geometry::scalar_t<Ts...>, N>;

template <class T0, class T1>
inline constexpr auto make_plane2d(const point<T0>& a, const point<T1>& b) {
    return plane_t<2, T0, T1>{perpendicular(b - a), a};
}

template <class T0, class T1, class T2>
inline constexpr auto make_plane3d(const point3d<T0>& a, const point3d<T1>& b,
                                   const point3d<T2>& c) {
    return plane_t<3, T0, T1, T2>{cross(b - a, c - a), a};
}

template <index N, class T>
inline void swap(plane<T, N>& lhs, plane<T, N>& rhs) { lhs.swap(rhs); }

}  // namespace ac
