// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/geometry/traits.hpp>
#include <actl/operation/scalar/all.hpp>
#include <actl/range/span.hpp>
#include <actl/utility/introspection.hpp>
#include <functional>

namespace ac {

/// N-dimensional point.
template<class T, index N = 2>
class point;

/// N-dimensional point base class implementing common functionality.
template<class T, index N>
class point_base : base<> {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = T const&;
    using pointer = T*;
    using const_pointer = T const*;

    template<class... Ts>
        requires(... && std::is_convertible_v<Ts, T>)
    constexpr point_base(Ts&&... xs)
        : m_coordinates{T{std::forward<Ts>(xs)}...} {}

    // TODO: make explicit when conversion is narrowing.
    template<class T1>
    constexpr point_base(point<T1, N> const& rhs) {
        for (index i = 0; i < N; ++i)
            m_coordinates[i] = static_cast<T>(rhs[i]);
    }

    constexpr T* data() {
        return m_coordinates;
    }

    constexpr T const* data() const {
        return m_coordinates;
    }

    static constexpr index size() {
        return N;
    }

    constexpr T& operator[](index i) {
        return span{*this}[i];
    }

    constexpr T const& operator[](index i) const {
        return span{*this}[i];
    }

    constexpr T& operator()(index i) {
        return (*this)[i];
    }

    constexpr T const& operator()(index i) const {
        return (*this)[i];
    }

    friend void swap(point_base& lhs, point_base& rhs) {
        using std::swap;
        for (index i = 0; i < N; ++i)
            swap(lhs[i], rhs[i]);
    }

private:
    INTROSPECT(span{m_coordinates})

    T m_coordinates[N];
};

template<class... Ts>
point(Ts&&...) -> point<geometry::scalar_t<Ts...>, sizeof...(Ts)>;

template<class T, index N>
class point : public point_base<T, N> {
public:
    using point_base<T, N>::point_base;
};

template<index N, class T>
struct geometry_traits<point<T, N>> {
    using tag = point_tag;
    using scalar = T;
    using point = point<T, N>;
    static constexpr index dimension = N;
};

template<class T, index N>
struct range_properties<point<T, N>> : default_range_properties {};

namespace detail {

template<class T, index N, class Operation, class... Points>
constexpr auto& apply(Operation op, point<T, N>& dst, Points const&... points) {
    for (index i = 0; i < N; ++i)
        op(dst[i], points[i]...);
    return dst;
}

template<index N, class Operation, class... Points>
constexpr auto apply(Operation op, Points const&... points) {
    point<decltype(op(points[0]...)), N> dst;
    for (index i = 0; i < N; ++i)
        dst[i] = op(points[i]...);
    return dst;
}

} // namespace detail

/* Vector operations */

template<index N, class T0, class T1 = T0>
constexpr auto& operator+=(point<T0, N>& lhs, point<T1, N> const& rhs) {
    return detail::apply([](T0& lhs, T1 const& src) { lhs += src; }, lhs, rhs);
}

template<index N, class T0, class T1 = T0>
constexpr auto& operator-=(point<T0, N>& lhs, point<T1, N> const& rhs) {
    return detail::apply([](T0& lhs, T1 const& src) { lhs -= src; }, lhs, rhs);
}

template<index N, class T0, class T1>
constexpr auto& operator*=(point<T0, N>& lhs, T1 const& factor) {
    return detail::apply([&factor](T0& lhs) { lhs *= factor; }, lhs);
}

template<index N, class T0, class T1>
constexpr auto& operator/=(point<T0, N>& lhs, T1 const& factor) {
    return detail::apply([&factor](T0& lhs) { lhs /= factor; }, lhs);
}

template<index N, class T>
constexpr auto operator-(point<T, N> const& src) {
    return detail::apply<N>(std::negate<T>{}, src);
}

template<index N, class T0, class T1>
constexpr auto operator+(point<T0, N> const& lhs, point<T1, N> const& rhs) {
    return detail::apply<N>(std::plus{}, lhs, rhs);
}

template<index N, class T0, class T1>
constexpr auto operator-(point<T0, N> const& lhs, point<T1, N> const& rhs) {
    return detail::apply<N>(std::minus{}, lhs, rhs);
}

template<index N, class T0, class T1>
constexpr bool perform(
    equal_t,
    Policy auto const& policy,
    point<T0, N> const& lhs,
    point<T1, N> const& rhs
) {
    return equal(policy, span{lhs}, span{rhs});
}

template<index N, class T0, class T1>
constexpr bool perform(
    less_t,
    Policy auto const& policy,
    point<T0, N> const& lhs,
    point<T1, N> const& rhs
) {
    return less(policy, span{lhs}, span{rhs});
}

template<index N, class T0, class T1>
constexpr auto perform(
    Mul, Policy auto const& policy, point<T0, N> const& lhs, T1 const& factor
) {
    return detail::apply<N>(
        [&policy, &factor](T0 const& x) { return mul(policy, x, factor); }, lhs
    );
}

template<index N, class T0, class T1>
constexpr auto perform(
    Mul, Policy auto const& policy, T0 const& factor, point<T1, N> const& rhs
) {
    return detail::apply<N>(
        [&policy, &factor](T1 const& x) { return mul(policy, factor, x); }, rhs
    );
}

template<index N, class T0, class T1>
constexpr auto perform(
    Div, Policy auto const& policy, point<T0, N> const& lhs, T1 const& factor
) {
    AC_ASSERT(!equal(policy, factor, 0));
    return detail::apply<N>(
        [&policy, &factor](T0 const& x) { return div(policy, x, factor); }, lhs
    );
}

template<index N, class T0, class T1>
constexpr auto dot(
    Policy auto const& policy, point<T0, N> const& lhs, point<T1, N> const& rhs
) {
    result_t<Mul, Policy, T0, T1> res = 0;
    for (index i = 0; i < N; ++i)
        res += product(policy, lhs[i], rhs[i]);
    return res;
}

template<index N, class T0, class T1>
constexpr auto dot(point<T0, N> const& lhs, point<T1, N> const& rhs) {
    return dot(default_policy, lhs, rhs);
}

template<index N, class T>
constexpr auto dot(Policy auto const& policy, point<T, N> const& p) {
    return dot(policy, p, p);
}

template<index N, class T>
constexpr auto dot(point<T, N> const& p) {
    return dot(default_policy, p);
}

template<index N, class T>
constexpr bool degenerate(Policy auto const& policy, point<T, N> const& p) {
    for (index i = 0; i < N; ++i) {
        if (!equal(policy, p[i], 0))
            return false;
    }
    return true;
}

template<class T>
constexpr bool degenerate(T const& x) {
    return degenerate(default_policy, x);
}

namespace detail {

template<class T, index N>
class point_xy : public point_base<T, N> {
public:
    using point_base<T, N>::point_base;

    constexpr T& x() {
        return (*this)[0];
    }

    constexpr T& y() {
        return (*this)[1];
    }

    constexpr T const& x() const {
        return (*this)[0];
    }

    constexpr T const& y() const {
        return (*this)[1];
    }
};

template<class T, index N>
class point_xyz : public point_xy<T, N> {
public:
    using point_xy<T, N>::point_xy;

    constexpr T& z() {
        return (*this)[2];
    }

    constexpr T const& z() const {
        return (*this)[2];
    }
};

} // namespace detail

/// 2-dimensional point specialization.
template<class T>
class point<T, 2> : public detail::point_xy<T, 2> {
public:
    using detail::point_xy<T, 2>::point_xy;
};

template<class T>
using point2d = point<T, 2>;

template<class T0, class T1>
constexpr bool y_compare(
    Policy auto const& policy, point<T0> const& lhs, point<T1> const& rhs
) {
    int v = cmp3way(policy, lhs[1], rhs[1]);
    return v < 0 || (v == 0 && less(policy, lhs[0], rhs[0]));
}

/// Point @p src rotated by pi/2 counter-clockwise.
template<class T>
constexpr point<T> perpendicular(point<T> const& src) {
    return point{-src[1], src[0]};
}

/// 3-dimensional point specialization.
template<class T>
class point<T, 3> : public detail::point_xyz<T, 3> {
public:
    using detail::point_xyz<T, 3>::point_xyz;
};

template<class T>
using point3d = point<T, 3>;

template<class T0, class T1>
constexpr auto cross(
    Policy auto const& policy, point3d<T0> const& lhs, point3d<T1> const& rhs
) {
    return point{
        product(policy, lhs[1], rhs[2]) - product(policy, lhs[2], rhs[1]),
        product(policy, lhs[2], rhs[0]) - product(policy, lhs[0], rhs[2]),
        product(policy, lhs[0], rhs[1]) - product(policy, lhs[1], rhs[0])
    };
}

template<class T0, class T1>
constexpr auto cross(point3d<T0> const& lhs, point3d<T1> const& rhs) {
    return cross(default_policy, lhs, rhs);
}

} // namespace ac
