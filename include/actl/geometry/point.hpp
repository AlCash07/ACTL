/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/functional/compare.hpp>
#include <actl/geometry/traits.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/util/introspection.hpp>
#include <actl/util/span.hpp>
#include <functional>
#include <initializer_list>

namespace ac {

/**
 * N-dimensional point.
 */
template <class T, index N = 2>
class point;

/**
 * N-dimensional point base class implementing common functionality.
 */
template <class T, index N>
class point_base : op::base<> {
public:
    template <class... Ts, enable_int_if<(... && std::is_convertible_v<Ts, T>)> = 0>
    constexpr point_base(Ts&&... xs) : coordinates_{T{std::forward<Ts>(xs)}...} {}

    template <class T1>
    explicit constexpr point_base(const point<T1, N>& rhs) {
        for (index i = 0; i < N; ++i) (*this)[i] = static_cast<T>(rhs[i]);
    }

    constexpr span<T, N> span() { return coordinates_; }
    constexpr cspan<T, N> span() const { return coordinates_; }

    constexpr T* data() { return coordinates_; }
    constexpr const T* data() const { return coordinates_; }

    constexpr T& operator[](index i) { return span()[i]; }
    constexpr const T& operator[](index i) const { return span()[i]; }

    constexpr T& operator()(index i) { return span()[i]; }
    constexpr const T& operator()(index i) const { return span()[i]; }

    explicit constexpr operator bool() const {
        for (index i = 0; i < N; ++i)
            if (span()[i]) return true;
        return false;
    }

    friend void swap(point_base& lhs, point_base& rhs) {
        using std::swap;
        for (index i = 0; i < N; ++i) swap(lhs[i], rhs[i]);
    }

private:
    // TODO: this causes point to look like ([x, y]) in pretty format. Fit it.
    INTROSPECT(coordinates_)

    T coordinates_[N];
};

template <class... Ts>
point(Ts&&...) -> point<geometry::scalar_t<Ts...>, sizeof...(Ts)>;

template <class T, index N>
class point : public point_base<T, N> {
public:
    using point_base<T, N>::point_base;
};

template <index N, class T>
struct geometry_traits<point<T, N>> {
    using tag    = point_tag;
    using scalar = T;
    using point  = point<T, N>;
    static constexpr index dimension = N;
};

namespace detail {

template <class T, index N, class Operation, class... Points>
inline constexpr auto& apply(Operation op, point<T, N>& dst, const Points&... points) {
    for (index i = 0; i < N; ++i)
        op(dst[i], points[i]...);
    return dst;
}

template <index N, class Operation, class... Points>
inline constexpr auto apply(Operation op, const Points&... points) {
    point<decltype(op(points[0]...)), N> dst;
    for (index i = 0; i < N; ++i)
        dst[i] = op(points[i]...);
    return dst;
}

}  // namespace detail

/* Vector operations */

template <index N, class T0, class T1 = T0>
inline constexpr auto& operator += (point<T0, N>& lhs, const point<T1, N>& rhs) {
    return detail::apply([](T0& lhs, const T1& src) { lhs += src; }, lhs, rhs);
}

template <index N, class T0, class T1 = T0>
inline constexpr auto& operator -= (point<T0, N>& lhs, const point<T1, N>& rhs) {
    return detail::apply([](T0& lhs, const T1& src) { lhs -= src; }, lhs, rhs);
}

template <index N, class T0, class T1>
inline constexpr auto& operator *= (point<T0, N>& lhs, const T1& factor) {
    return detail::apply([&factor](T0& lhs) { lhs *= factor; }, lhs);
}

template <index N, class T0, class T1>
inline constexpr auto& operator /= (point<T0, N>& lhs, const T1& factor) {
    return detail::apply([&factor](T0& lhs) { lhs /= factor; }, lhs);
}

template <index N, class T>
inline constexpr auto operator -(const point<T, N>& src) {
    return detail::apply<N>(std::negate<T>{}, src);
}

template <index N, class T0, class T1>
inline constexpr auto operator + (const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return detail::apply<N>(std::plus{}, lhs, rhs);
}

template <index N, class T0, class T1>
inline constexpr auto operator - (const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return detail::apply<N>(std::minus{}, lhs, rhs);
}

template <index N, class T0, class T1>
inline constexpr auto operator * (const point<T0, N>& lhs, const T1& factor) {
    return detail::apply<N>([&factor](const T0& x) { return x * factor; }, lhs);
}

template <index N, class T0, class T1>
inline constexpr auto operator * (const T0& factor, const point<T1, N>& rhs) {
    return detail::apply<N>([&factor](const T1& x) { return factor * x; }, rhs);
}

template <index N, class T0, class T1>
inline constexpr auto operator / (const point<T0, N>& lhs, const T1& factor) {
    ACTL_ASSERT(factor != T1{0});
    return detail::apply<N>([&factor](const T0& x) { return x / factor; }, lhs);
}

template <class P = use_default, index N, class T0, class T1>
inline constexpr auto dot(const point<T0, N>& lhs, const point<T1, N>& rhs) {
    using R = deduce_t<P, geometry::scalar_t<T0, T1>>;
    R res = 0;
    for (index i = 0; i < N; ++i)
        res += static_cast<R>(lhs[i]) * rhs[i];
    return res;
}

template <class P = use_default, index N, class T>
inline constexpr auto dot(const point<T, N>& p) {
    return dot<P>(p, p);
}

/* Comparison */

namespace op {

template <class Policy, index N, class T0, class T1>
inline constexpr bool equal(Policy&& policy, const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return op::equal(policy, lhs.span(), rhs.span());
}

template <class Policy, index N, class T0, class T1>
inline constexpr bool less(Policy&& policy, const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return less(policy, lhs.span(), rhs.span());
}

}  // namespace op

}  // namespace ac
