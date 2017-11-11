/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/geometry/traits/geometry_traits.hpp>
#include <actl/range/algorithm.hpp>
#include <actl/type/use_default.hpp>
#include <initializer_list>

namespace ac {

/**
 * N-dimensional point.
 */
template <int N, class T>
class point;

/**
 * N-dimensional point base class implementing common functionality.
 */
template <int N, class T>
class point_base {
public:
    template <class... Ts>
    explicit constexpr point_base(Ts&&... values) : coordinates_{std::forward<Ts>(values)...} {}

    constexpr point_base(std::initializer_list<T> values) {
        ACTL_ASSERT(values.size() == N);
        copy(values, coordinates_);
    }

    template <class T1>
    explicit constexpr point_base(const point<N, T1>& other) { (*this) = other; }

    template <class T1>
    explicit constexpr point_base(point<N, T1>&& other) { (*this) = other; }

    constexpr T*       data()       { return coordinates_; }
    constexpr const T* data() const { return coordinates_; }

    constexpr T& operator [] (int index) {
        ACTL_ASSERT(0 <= index && index < N);
        return data()[index];
    }

    constexpr T& operator () (int index) { return data()[index]; }

    constexpr const T& operator [] (int index) const {
        ACTL_ASSERT(0 <= index && index < N);
        return data()[index];
    }

    constexpr const T& operator () (int index) const { return data()[index]; }

    explicit constexpr operator bool () const {
        for (int i = 0; i < N; ++i)
            if (data()[i]) return true;
        return false;
    }

    void swap(point_base& other) {
        using std::swap;
        for (int i = 0; i < N; ++i)
            swap(data()[i], other[i]);
    }

    template <class T1>
    constexpr point_base& operator = (const point_base<N, T1>& other) {
        return apply([](T& dst, const T1& src) { dst = static_cast<T>(src); }, other);
    }

    template <class T1>
    constexpr point_base& operator += (const point_base<N, T1>& other) {
        return apply([](T& dst, const T1& src) { dst += src; }, other);
    }

    template <class T1>
    constexpr point_base& operator -= (const point_base<N, T1>& other) {
        return apply([](T& dst, const T1& src) { dst -= src; }, other);
    }

    template <class T1>
    constexpr point_base& operator *= (const T1& factor) {
        return apply([&factor](T& dst) { dst *= factor; });
    }

    template <class T1>
    constexpr point_base& operator /= (const T1& factor) {
        return apply([&factor](T& dst) { dst /= factor; });
    }

private:
    template <class Operation, class... Points>
    constexpr point_base& apply(Operation op, Points&&... points) {
        for (int i = 0; i < N; ++i)
            op(data()[i], points[i]...);
        return *this;
    }

    T coordinates_[N];
};

template <int N, class T>
class point : public point_base<N, T> {
public:
    using point_base<N, T>::point_base;
};

template <int N, class T>
struct geometry_traits<point<N, T>> {
    using tag    = point_tag;
    using scalar = T;
    using point  = point<N, T>;
    static constexpr int dimension = N;
};

template <int N, class... Ts>
using point_type = point<N, geometry::scalar_t<Ts...>>;

template <class... Ts>
inline constexpr auto make_point(Ts&&... values) {
    return point_type<sizeof...(Ts), Ts...>(std::forward<Ts>(values)...);
}

template <int N, class T>
inline void swap(point<N, T>& lhs, point<N, T>& rhs) { lhs.swap(rhs); }

template <class Device, int N, class T>
inline bool read(Device& in, point<N, T>& arg) {
    return read(in, arg.data(), N);
}

template <class Device, int N, class T>
inline int write(Device& out, const point<N, T>& arg) {
    return write(out, arg.data(), N);
}

namespace detail {

template <int N, class Operation, class... Points>
inline constexpr auto apply(Operation op, const Points&... points) {
    point_type<N, decltype(op(points[0]...))> dst;
    for (int i = 0; i < N; ++i)
        dst[i] = op(points[i]...);
    return dst;
}

}  // namespace detail

/* Vector operations */

template <int N, class T>
inline constexpr auto operator - (const point<N, T>& src) {
    return detail::apply<N>([](const T& x) { return -x; }, src);
}

template <int N, class T0, class T1>
inline constexpr auto operator + (const point<N, T0>& lhs, const point<N, T1>& rhs) {
    return detail::apply<N>([](const T0& x, const T1& y) { return x + y; }, lhs, rhs);
}

template <int N, class T0, class T1>
inline constexpr auto operator - (const point<N, T0>& lhs, const point<N, T1>& rhs) {
    return detail::apply<N>([](const T0& x, const T1& y) { return x - y; }, lhs, rhs);
}

template <int N, class T0, class T1>
inline constexpr auto operator * (const point<N, T0>& lhs, const T1& factor) {
    return detail::apply<N>([&factor](const T0& x) { return x * factor; }, lhs);
}

template <int N, class T0, class T1>
inline constexpr auto operator * (const T0& factor, const point<N, T1>& rhs) {
    return detail::apply<N>([&factor](const T1& x) { return factor * x; }, rhs);
}

template <int N, class T0, class T1>
inline constexpr auto operator / (const point<N, T0>& lhs, const T1& factor) {
    ACTL_ASSERT(factor != T1{0});
    return detail::apply<N>([&factor](const T0& x) { return x / factor; }, lhs);
}

template <class P = use_default, int N, class T0, class T1,
          class X = deduce_type_t<P, geometry::scalar_t<T0, T1>>>
inline constexpr X dot(const point<N, T0>& lhs, const point<N, T1>& rhs) {
    X res{0};
    for (int i = 0; i < N; ++i)
        res += static_cast<X>(lhs[i]) * rhs[i];
    return res;
}

template <class P = use_default, int N, class T>
inline constexpr auto abs(const point<N, T>& src) {
    return dot<P>(src, src);
}

/* Comparison operators */

template <int N, class T0, class T1>
inline constexpr bool operator == (const point<N, T0>& lhs, const point<N, T1>& rhs) {
    return std::equal(lhs.data(), lhs.data() + N, rhs.data());
}

template <int N, class T0, class T1>
inline constexpr bool operator != (const point<N, T0>& lhs, const point<N, T1>& rhs) {
    return !(lhs == rhs);
}

template <int N, class T0, class T1>
inline constexpr bool operator < (const point<N, T0>& lhs, const point<N, T1>& rhs) {
    return std::lexicographical_compare(lhs.data(), lhs.data() + N, rhs.data(), rhs.data() + N);
}

template <int N, class T0, class T1>
inline constexpr bool operator > (const point<N, T0>& lhs, const point<N, T1>& rhs) {
    return rhs < lhs;
}

template <int N, class T0, class T1>
inline constexpr bool operator <= (const point<N, T0>& lhs, const point<N, T1>& rhs) {
    return !(lhs > rhs);
}

template <int N, class T0, class T1>
inline constexpr bool operator >= (const point<N, T0>& lhs, const point<N, T1>& rhs) {
    return !(lhs < rhs);
}

}  // namespace ac
