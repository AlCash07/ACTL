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
// TODO: add bool parameter indicating if the inside is included, i.e. we have a disk.
template <class T, index N = 3>
class sphere {
public:
    point<T, N> center;
    T radius;

    constexpr sphere() = default;

    template <class T1, class T2>
    explicit constexpr sphere(const point<T1, N>& center, const T2& radius)
        : center{center}, radius{static_cast<T>(radius)} {}

    template <class T1>
    explicit constexpr sphere(const sphere<T1, N>& rhs) : sphere{rhs.center, rhs.radius} {}

    explicit constexpr operator bool() const { return radius > T{0}; }

    friend void swap(sphere& lhs, sphere& rhs) {
        using std::swap;
        swap(lhs.center, rhs.center);
        swap(lhs.radius, rhs.radius);
    }

private:
    INTROSPECT(center, radius)
};

template <index N, class T0, class T1>
sphere(const point<T0, N>&, const T1&) -> sphere<geometry::scalar_t<T0, T1>, N>;

template <index N, class T>
struct geometry_traits<sphere<T, N>> : geometry_traits_base<sphere_tag, point<T, N>> {};

}  // namespace ac
