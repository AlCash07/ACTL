// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/geometry/traits.hpp>
#include <actl/operation/scalar/all.hpp>
#include <actl/range/span.hpp>
#include <actl/utility/introspection.hpp>
#include <functional>

namespace ac {

/// N-dimensional point.
template <class T, index N = 2>
class point;

/// N-dimensional point base class implementing common functionality.
template <class T, index N>
class point_base : base<>
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    template <
        class... Ts,
        enable_int_if<(... && std::is_convertible_v<Ts, T>)> = 0>
    constexpr point_base(Ts&&... xs) : coordinates_{T{std::forward<Ts>(xs)}...}
    {}

    // TODO: make explicit when conversion is narrowing.
    template <class T1>
    constexpr point_base(const point<T1, N>& rhs)
    {
        for (index i = 0; i < N; ++i)
            coordinates_[i] = static_cast<T>(rhs[i]);
    }

    constexpr T* data()
    {
        return coordinates_;
    }

    constexpr const T* data() const
    {
        return coordinates_;
    }

    static constexpr index size()
    {
        return N;
    }

    constexpr T& operator[](index i)
    {
        return span{*this}[i];
    }

    constexpr const T& operator[](index i) const
    {
        return span{*this}[i];
    }

    constexpr T& operator()(index i)
    {
        return (*this)[i];
    }

    constexpr const T& operator()(index i) const
    {
        return (*this)[i];
    }

    friend void swap(point_base& lhs, point_base& rhs)
    {
        using std::swap;
        for (index i = 0; i < N; ++i)
            swap(lhs[i], rhs[i]);
    }

private:
    INTROSPECT(span{coordinates_})

    T coordinates_[N];
};

template <class... Ts>
point(Ts&&...) -> point<geometry::scalar_t<Ts...>, sizeof...(Ts)>;

template <class T, index N>
class point : public point_base<T, N>
{
public:
    using point_base<T, N>::point_base;
};

template <index N, class T>
struct geometry_traits<point<T, N>>
{
    using tag = point_tag;
    using scalar = T;
    using point = point<T, N>;
    static constexpr index dimension = N;
};

template <class T, index N>
struct range_traits<point<T, N>> : default_range_traits
{
    static constexpr index static_size = N;
};

namespace detail {

template <class T, index N, class Operation, class... Points>
constexpr auto& apply(Operation op, point<T, N>& dst, const Points&... points)
{
    for (index i = 0; i < N; ++i)
        op(dst[i], points[i]...);
    return dst;
}

template <index N, class Operation, class... Points>
constexpr auto apply(Operation op, const Points&... points)
{
    point<decltype(op(points[0]...)), N> dst;
    for (index i = 0; i < N; ++i)
        dst[i] = op(points[i]...);
    return dst;
}

} // namespace detail

/* Vector operations */

template <index N, class T0, class T1 = T0>
constexpr auto& operator+=(point<T0, N>& lhs, const point<T1, N>& rhs)
{
    return detail::apply([](T0& lhs, const T1& src) { lhs += src; }, lhs, rhs);
}

template <index N, class T0, class T1 = T0>
constexpr auto& operator-=(point<T0, N>& lhs, const point<T1, N>& rhs)
{
    return detail::apply([](T0& lhs, const T1& src) { lhs -= src; }, lhs, rhs);
}

template <index N, class T0, class T1>
constexpr auto& operator*=(point<T0, N>& lhs, const T1& factor)
{
    return detail::apply([&factor](T0& lhs) { lhs *= factor; }, lhs);
}

template <index N, class T0, class T1>
constexpr auto& operator/=(point<T0, N>& lhs, const T1& factor)
{
    return detail::apply([&factor](T0& lhs) { lhs /= factor; }, lhs);
}

template <index N, class T>
constexpr auto operator-(const point<T, N>& src)
{
    return detail::apply<N>(std::negate<T>{}, src);
}

template <index N, class T0, class T1>
constexpr auto operator+(const point<T0, N>& lhs, const point<T1, N>& rhs)
{
    return detail::apply<N>(std::plus{}, lhs, rhs);
}

template <index N, class T0, class T1>
constexpr auto operator-(const point<T0, N>& lhs, const point<T1, N>& rhs)
{
    return detail::apply<N>(std::minus{}, lhs, rhs);
}

template <class Policy, index N, class T0, class T1>
constexpr bool perform(
    equal_t,
    const Policy& policy,
    const point<T0, N>& lhs,
    const point<T1, N>& rhs)
{
    return equal(policy, span{lhs}, span{rhs});
}

template <class Policy, index N, class T0, class T1>
constexpr bool perform(
    less_t,
    const Policy& policy,
    const point<T0, N>& lhs,
    const point<T1, N>& rhs)
{
    return less(policy, span{lhs}, span{rhs});
}

template <class Policy, index N, class T0, class T1>
constexpr auto perform(
    Mul, const Policy& policy, const point<T0, N>& lhs, const T1& factor)
{
    return detail::apply<N>(
        [&policy, &factor](const T0& x) { return mul(policy, x, factor); },
        lhs);
}

template <class Policy, index N, class T0, class T1>
constexpr auto perform(
    Mul, const Policy& policy, const T0& factor, const point<T1, N>& rhs)
{
    return detail::apply<N>(
        [&policy, &factor](const T1& x) { return mul(policy, factor, x); },
        rhs);
}

template <class Policy, index N, class T0, class T1>
constexpr auto perform(
    Div, const Policy& policy, const point<T0, N>& lhs, const T1& factor)
{
    ACTL_ASSERT(!equal(policy, factor, 0));
    return detail::apply<N>(
        [&policy, &factor](const T0& x) { return div(policy, x, factor); },
        lhs);
}

template <class Policy, index N, class T0, class T1>
constexpr auto dot(
    const Policy& policy, const point<T0, N>& lhs, const point<T1, N>& rhs)
{
    result_t<Mul, Policy, T0, T1> res = 0;
    for (index i = 0; i < N; ++i)
        res += product(policy, lhs[i], rhs[i]);
    return res;
}

template <index N, class T0, class T1>
constexpr auto dot(const point<T0, N>& lhs, const point<T1, N>& rhs)
{
    return dot(default_policy, lhs, rhs);
}

template <
    class Policy,
    index N,
    class T,
    enable_int_if<is_policy_v<Policy>> = 0>
constexpr auto dot(const Policy& policy, const point<T, N>& p)
{
    return dot(policy, p, p);
}

template <index N, class T>
constexpr auto dot(const point<T, N>& p)
{
    return dot(default_policy, p);
}

template <class Policy, index N, class T>
constexpr bool degenerate(const Policy& policy, const point<T, N>& p)
{
    for (index i = 0; i < N; ++i)
    {
        if (!equal(policy, p[i], 0))
            return false;
    }
    return true;
}

template <class T>
constexpr bool degenerate(const T& x)
{
    return degenerate(default_policy, x);
}

namespace detail {

template <class T, index N>
class point_xy : public point_base<T, N>
{
public:
    using point_base<T, N>::point_base;

    constexpr T& x()
    {
        return (*this)[0];
    }

    constexpr T& y()
    {
        return (*this)[1];
    }

    constexpr const T& x() const
    {
        return (*this)[0];
    }

    constexpr const T& y() const
    {
        return (*this)[1];
    }
};

template <class T, index N>
class point_xyz : public point_xy<T, N>
{
public:
    using point_xy<T, N>::point_xy;

    constexpr T& z()
    {
        return (*this)[2];
    }

    constexpr const T& z() const
    {
        return (*this)[2];
    }
};

} // namespace detail

/// 2-dimensional point specialization.
template <class T>
class point<T, 2> : public detail::point_xy<T, 2>
{
public:
    using detail::point_xy<T, 2>::point_xy;
};

template <class T>
using point2d = point<T, 2>;

template <class Policy, class T0, class T1>
constexpr bool y_compare(
    const Policy& policy, const point<T0>& lhs, const point<T1>& rhs)
{
    int v = cmp3way(policy, lhs[1], rhs[1]);
    return v < 0 || (v == 0 && less(policy, lhs[0], rhs[0]));
}

/// Point @p src rotated by pi/2 counter-clockwise.
template <class T>
constexpr point<T> perpendicular(const point<T>& src)
{
    return point{-src[1], src[0]};
}

/// 3-dimensional point specialization.
template <class T>
class point<T, 3> : public detail::point_xyz<T, 3>
{
public:
    using detail::point_xyz<T, 3>::point_xyz;
};

template <class T>
using point3d = point<T, 3>;

template <class Policy, class T0, class T1>
constexpr auto cross(
    const Policy& policy, const point3d<T0>& lhs, const point3d<T1>& rhs)
{
    return point{
        product(policy, lhs[1], rhs[2]) - product(policy, lhs[2], rhs[1]),
        product(policy, lhs[2], rhs[0]) - product(policy, lhs[0], rhs[2]),
        product(policy, lhs[0], rhs[1]) - product(policy, lhs[1], rhs[0])};
}

template <class T0, class T1>
constexpr auto cross(const point3d<T0>& lhs, const point3d<T1>& rhs)
{
    return cross(default_policy, lhs, rhs);
}

} // namespace ac
