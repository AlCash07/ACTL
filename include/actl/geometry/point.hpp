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
template<typename T, index N = 2>
class point;

/// N-dimensional point base class implementing common functionality.
template<typename T, index N>
class point_base : base<> {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = T const&;
    using pointer = T*;
    using const_pointer = T const*;

    template<typename... Ts>
        requires(... && std::is_convertible_v<Ts, T>)
    constexpr point_base(Ts&&... xs)
        : m_coordinates{T{std::forward<Ts>(xs)}...} {}

    // TODO: make explicit when conversion is narrowing.
    template<typename TR>
    constexpr point_base(point<TR, N> const& r) {
        for (index i = 0; i < N; ++i)
            m_coordinates[i] = static_cast<T>(r[i]);
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

    friend void swap(point_base& l, point_base& r) {
        using std::swap;
        for (index i = 0; i < N; ++i)
            swap(l[i], r[i]);
    }

private:
    INTROSPECT(span{m_coordinates})

    T m_coordinates[N];
};

template<typename... Ts>
point(Ts&&...) -> point<geometry::scalar_t<Ts...>, sizeof...(Ts)>;

template<typename T, index N>
class point : public point_base<T, N> {
public:
    using point_base<T, N>::point_base;
};

template<index N, typename T>
struct geometry_traits<point<T, N>> {
    using tag = point_tag;
    using scalar = T;
    using point = point<T, N>;
    static constexpr index dimension = N;
};

template<typename T, index N>
struct range_properties<point<T, N>> : default_range_properties {};

namespace detail {

template<typename T, index N, typename Operation, typename... Points>
constexpr auto& apply(
    Operation op, point<T, N>& output, Points const&... points
) {
    for (index i = 0; i < N; ++i)
        op(output[i], points[i]...);
    return output;
}

template<index N, typename Operation, typename... Points>
constexpr auto apply(Operation op, Points const&... points) {
    point<decltype(op(points[0]...)), N> output;
    for (index i = 0; i < N; ++i)
        output[i] = op(points[i]...);
    return output;
}

} // namespace detail

/* Vector operations */

template<index N, typename TL, typename TR = TL>
constexpr auto& operator+=(point<TL, N>& l, point<TR, N> const& r) {
    return detail::apply([](TL& l, TR const& x) { l += x; }, l, r);
}

template<index N, typename TL, typename TR = TL>
constexpr auto& operator-=(point<TL, N>& l, point<TR, N> const& r) {
    return detail::apply([](TL& l, TR const& x) { l -= x; }, l, r);
}

template<index N, typename TL, typename TR>
constexpr auto& operator*=(point<TL, N>& l, TR const& factor) {
    return detail::apply([&factor](TL& l) { l *= factor; }, l);
}

template<index N, typename TL, typename TR>
constexpr auto& operator/=(point<TL, N>& l, TR const& factor) {
    return detail::apply([&factor](TL& l) { l /= factor; }, l);
}

template<index N, typename T>
constexpr auto operator-(point<T, N> const& p) {
    return detail::apply<N>(std::negate<T>{}, p);
}

template<index N, typename TL, typename TR>
constexpr auto operator+(point<TL, N> const& l, point<TR, N> const& r) {
    return detail::apply<N>(std::plus{}, l, r);
}

template<index N, typename TL, typename TR>
constexpr auto operator-(point<TL, N> const& l, point<TR, N> const& r) {
    return detail::apply<N>(std::minus{}, l, r);
}

template<index N, typename TL, typename TR>
constexpr bool perform(
    equal_t,
    Policy auto const& policy,
    point<TL, N> const& l,
    point<TR, N> const& r
) {
    return equal(policy, span{l}, span{r});
}

template<index N, typename TL, typename TR>
constexpr bool perform(
    less_t,
    Policy auto const& policy,
    point<TL, N> const& l,
    point<TR, N> const& r
) {
    return less(policy, span{l}, span{r});
}

template<index N, typename TL, typename TR>
constexpr auto perform(
    Mul, Policy auto const& policy, point<TL, N> const& l, TR const& factor
) {
    return detail::apply<N>(
        [&policy, &factor](TL const& x) { return mul(policy, x, factor); }, l
    );
}

template<index N, typename TL, typename TR>
constexpr auto perform(
    Mul, Policy auto const& policy, TL const& factor, point<TR, N> const& r
) {
    return detail::apply<N>(
        [&policy, &factor](TR const& x) { return mul(policy, factor, x); }, r
    );
}

template<index N, typename TL, typename TR>
constexpr auto perform(
    Div, Policy auto const& policy, point<TL, N> const& l, TR const& factor
) {
    AC_ASSERT(!equal(policy, factor, 0));
    return detail::apply<N>(
        [&policy, &factor](TL const& x) { return div(policy, x, factor); }, l
    );
}

template<index N, typename TL, typename TR>
constexpr auto dot(
    Policy auto const& policy, point<TL, N> const& l, point<TR, N> const& r
) {
    result_t<Mul, Policy, TL, TR> res = 0;
    for (index i = 0; i < N; ++i)
        res += product(policy, l[i], r[i]);
    return res;
}

template<index N, typename TL, typename TR>
constexpr auto dot(point<TL, N> const& l, point<TR, N> const& r) {
    return dot(default_policy, l, r);
}

template<index N, typename T>
constexpr auto dot(Policy auto const& policy, point<T, N> const& p) {
    return dot(policy, p, p);
}

template<index N, typename T>
constexpr auto dot(point<T, N> const& p) {
    return dot(default_policy, p);
}

template<index N, typename T>
constexpr bool degenerate(Policy auto const& policy, point<T, N> const& p) {
    for (index i = 0; i < N; ++i) {
        if (!equal(policy, p[i], 0))
            return false;
    }
    return true;
}

template<typename T>
constexpr bool degenerate(T const& x) {
    return degenerate(default_policy, x);
}

namespace detail {

template<typename T, index N>
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

template<typename T, index N>
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
template<typename T>
class point<T, 2> : public detail::point_xy<T, 2> {
public:
    using detail::point_xy<T, 2>::point_xy;
};

template<typename T>
using point2d = point<T, 2>;

template<typename TL, typename TR>
constexpr bool y_compare(
    Policy auto const& policy, point<TL> const& l, point<TR> const& r
) {
    int v = cmp3way(policy, l[1], r[1]);
    return v < 0 || (v == 0 && less(policy, l[0], r[0]));
}

/// Point @p p rotated by pi/2 counter-clockwise.
template<typename T>
constexpr point<T> perpendicular(point<T> const& p) {
    return point{-p[1], p[0]};
}

/// 3-dimensional point specialization.
template<typename T>
class point<T, 3> : public detail::point_xyz<T, 3> {
public:
    using detail::point_xyz<T, 3>::point_xyz;
};

template<typename T>
using point3d = point<T, 3>;

template<typename TL, typename TR>
constexpr auto cross(
    Policy auto const& policy, point3d<TL> const& l, point3d<TR> const& r
) {
    return point{
        product(policy, l[1], r[2]) - product(policy, l[2], r[1]),
        product(policy, l[2], r[0]) - product(policy, l[0], r[2]),
        product(policy, l[0], r[1]) - product(policy, l[1], r[0])
    };
}

template<typename TL, typename TR>
constexpr auto cross(point3d<TL> const& l, point3d<TR> const& r) {
    return cross(default_policy, l, r);
}

} // namespace ac
