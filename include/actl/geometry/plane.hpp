// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

/// Plane with equation in the form: dot(normal, x) = d.
/// In 2D represents a line.
template <class T, index N = 3>
class plane {
public:
    point<T, N> normal;  // normal vector
    T d;                 // distance from origin to the plane times the norm of normal

    constexpr plane() = default;

    template <class T1 = T, class T2 = T>
    explicit constexpr plane(const point<T1, N>& normal, const T2& d)
        : normal{normal}, d{static_cast<T>(d)} {}

    template <class T1 = T, class T2 = T>
    explicit constexpr plane(const point<T1, N>& normal, const point<T2, N>& p)
        : plane{normal, dot(cast_before<Mul, T>{}, normal, p)} {}

    template <class T1>
    explicit constexpr plane(const plane<T1, N>& rhs) : plane{rhs.normal, rhs.d} {}

    friend void swap(plane& lhs, plane& rhs) {
        using std::swap;
        swap(lhs.normal, rhs.normal);
        swap(lhs.d, rhs.d);
    }

    // Oriented distance from @p point to the plane times the norm of normal.
    template <class Policy, class T1 = T>
    constexpr auto operator()(const Policy& policy, const point<T1, N>& p) const {
        return dot(policy, normal, p) - d;
    }

    template <class T1 = T>
    constexpr auto operator()(const point<T1, N>& p) const {
        return (*this)(default_policy, p);
    }

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
constexpr auto make_plane2d(const point<T0>& a, const point<T1>& b) {
    return plane_t<2, T0, T1>{perpendicular(b - a), a};
}

template <class T0, class T1, class T2>
constexpr auto make_plane3d(const point3d<T0>& a, const point3d<T1>& b, const point3d<T2>& c) {
    return plane_t<3, T0, T1, T2>{cross(b - a, c - a), a};
}

template <class Policy, index N, class T>
constexpr bool degenerate(const Policy& policy, const plane<T, N>& pl) {
    return degenerate(policy, pl.normal);
}

}  // namespace ac
