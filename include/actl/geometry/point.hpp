/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/functional/arithmetic.hpp>
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
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    template <class... Ts, enable_int_if<(... && std::is_convertible_v<Ts, T>)> = 0>
    constexpr point_base(Ts&&... xs) : coordinates_{T{std::forward<Ts>(xs)}...} {}

    template <class T1>
    explicit constexpr point_base(const point<T1, N>& rhs) {
        for (index i = 0; i < N; ++i) coordinates_[i] = static_cast<T>(rhs[i]);
    }

    constexpr T* data() { return coordinates_; }
    constexpr const T* data() const { return coordinates_; }

    static constexpr index size() { return N; }

    constexpr T& operator[](index i) { return span{*this}[i]; }
    constexpr const T& operator[](index i) const { return span{*this}[i]; }

    constexpr T& operator()(index i) { return (*this)[i]; }
    constexpr const T& operator()(index i) const { return (*this)[i]; }

    explicit constexpr operator bool() const {
        for (index i = 0; i < N; ++i)
            if ((*this)[i]) return true;
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

template <class T, index N>
struct static_size<point<T, N>> : index_constant<N> {};

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

namespace op {

template <class Policy, index N, class T0, class T1>
inline constexpr bool equal(Policy&& policy, const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return op::equal(policy, span{lhs}, span{rhs});
}

template <class Policy, index N, class T0, class T1>
inline constexpr bool less(Policy&& policy, const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return less(policy, span{lhs}, span{rhs});
}

template <class Policy, index N, class T0, class T1>
inline constexpr auto product(Policy&& policy, const point<T0, N>& lhs, const T1& factor) {
    return detail::apply<N>([&policy, &factor](const T0& x) { return product(policy, x, factor); },
                            lhs);
}

template <class Policy, index N, class T0, class T1>
inline constexpr auto product(Policy&& policy, const T0& factor, const point<T1, N>& rhs) {
    return detail::apply<N>([&policy, &factor](const T1& x) { return product(policy, factor, x); },
                            rhs);
}

template <class Policy, index N, class T0, class T1>
inline constexpr auto ratio(Policy&& policy, const point<T0, N>& lhs, const T1& factor) {
    ACTL_ASSERT(!equal(policy, factor, T1{0}));
    return detail::apply<N>([&policy, &factor](const T0& x) { return ratio(policy, x, factor); },
                            lhs);
}

}  // namespace op

template <class Policy, index N, class T0, class T1>
inline constexpr auto dot(Policy&& policy, const point<T0, N>& lhs, const point<T1, N>& rhs) {
    op::product_t<Policy, T0, T1> res = 0;
    for (index i = 0; i < N; ++i) res += product(policy, lhs[i], rhs[i]);
    return res;
}

template <index N, class T0, class T1>
inline constexpr auto dot(const point<T0, N>& lhs, const point<T1, N>& rhs) {
    return dot(default_policy, lhs, rhs);
}

template <class Policy, index N, class T, enable_int_if<is_policy_v<Policy>> = 0>
inline constexpr auto dot(Policy&& policy, const point<T, N>& p) {
    return dot(policy, p, p);
}

template <index N, class T>
inline constexpr auto dot(const point<T, N>& p) {
    return dot(default_policy, p);
}

}  // namespace ac
