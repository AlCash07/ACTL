// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/point.hpp>

namespace ac {

/// N-dimensional sphere.
// TODO: add bool parameter indicating if the inside is included, i.e. we have a
// disk.
template <class T, index N = 3>
class sphere
{
public:
    point<T, N> center;
    T radius;

    constexpr sphere() = default;

    template <class T1 = T, class T2 = T>
    explicit constexpr sphere(point<T1, N> const& center, T2 const& radius)
        : center{center}, radius{static_cast<T>(radius)}
    {}

    template <class T1>
    explicit constexpr sphere(sphere<T1, N> const& rhs)
        : sphere{rhs.center, rhs.radius}
    {}

    friend void swap(sphere& lhs, sphere& rhs)
    {
        using std::swap;
        swap(lhs.center, rhs.center);
        swap(lhs.radius, rhs.radius);
    }

private:
    INTROSPECT(center, radius)
};

template <index N, class T0, class T1>
sphere(point<T0, N> const&, T1 const&) -> sphere<geometry::scalar_t<T0, T1>, N>;

template <index N, class T>
struct geometry_traits<sphere<T, N>>
    : geometry_traits_base<sphere_tag, point<T, N>>
{};

template <index N, class T>
constexpr bool degenerate(Policy auto const& policy, sphere<T, N> const& s)
{
    return !less(policy, 0, s.radius);
}

/* circle */

template <class T>
using circle = sphere<T, 2>;

template <class T0, class T1>
constexpr auto make_circle(point<T0> const& center, T1 const& radius)
{
    return circle<geometry::scalar_t<T0, T1>>{center, radius};
}

// Policy to indicate that polar angle is expected instead of a point.
template <class P>
struct polar_angle_policy : virtual policy
{
    explicit polar_angle_policy(P const& x) : policy{x} {}

    P const& policy;
};

} // namespace ac
