// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

/// Plane with equation in the form: dot(normal, x) = d.
/// In 2D represents a line.
template<typename T, index N = 3>
class plane {
public:
    point<T, N> normal; // normal vector
    T d; // distance from origin to the plane times the norm of normal

    constexpr plane() = default;

    template<typename T1 = T, typename T2 = T>
    explicit constexpr plane(point<T1, N> const& normal, T2 const& d)
        : normal{normal}, d{static_cast<T>(d)} {}

    template<typename T1 = T, typename T2 = T>
    explicit constexpr plane(point<T1, N> const& normal, point<T2, N> const& p)
        : plane{normal, dot(cast_before<Mul, T>{}, normal, p)} {}

    template<typename T1>
    explicit constexpr plane(plane<T1, N> const& r) : plane{r.normal, r.d} {}

    friend void swap(plane& l, plane& r) {
        using std::swap;
        swap(l.normal, r.normal);
        swap(l.d, r.d);
    }

    // Oriented distance from @p point to the plane times the norm of normal.
    template<typename T1 = T>
    constexpr auto operator()(Policy auto const& policy, point<T1, N> const& p)
        const {
        return dot(policy, normal, p) - d;
    }

    template<typename T1 = T>
    constexpr auto operator()(point<T1, N> const& p) const {
        return (*this)(default_policy, p);
    }

private:
    INTROSPECT(normal, d)
};

template<index N, typename T0, typename T1>
plane(point<T0, N> const&, T1 const&) -> plane<geometry::scalar_t<T0, T1>, N>;

template<index N, typename T0, typename T1>
plane(point<T0, N> const&, point<T1, N> const&)
    -> plane<geometry::scalar_t<T0, T1>, N>;

template<index N, typename T>
struct geometry_traits<plane<T, N>>
    : geometry_traits_base<plane_tag, point<T, N>> {};

template<index N, typename... Ts>
using plane_t = plane<geometry::scalar_t<Ts...>, N>;

template<typename T0, typename T1>
constexpr auto make_plane2d(point<T0> const& a, point<T1> const& b) {
    return plane_t<2, T0, T1>{perpendicular(b - a), a};
}

template<typename T0, typename T1, typename T2>
constexpr auto make_plane3d(
    point3d<T0> const& a, point3d<T1> const& b, point3d<T2> const& c
) {
    return plane_t<3, T0, T1, T2>{cross(b - a, c - a), a};
}

template<index N, typename T>
constexpr bool degenerate(Policy auto const& policy, plane<T, N> const& pl) {
    return degenerate(policy, pl.normal);
}

} // namespace ac
