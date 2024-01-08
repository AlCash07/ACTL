/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at
 *http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/linear_algebra/vector.hpp>

namespace ac::math {

template <class Policy, index N, class T>
inline constexpr bool degenerate(const Policy& policy, const vec<T, N>& p)
{
    for (index i = 0; i < N; ++i)
    {
        if (!equal(policy, p[i], 0))
            return false;
    }
    return true;
}

template <class T>
inline constexpr bool degenerate(const T& x)
{
    return degenerate(default_policy, x);
}

template <class Policy, class T0, class T1>
inline constexpr bool y_compare(
    const Policy& policy, const vec<T0>& lhs, const vec<T1>& rhs)
{
    int v = cmp3way(policy, lhs[1], rhs[1]);
    return v < 0 || (v == 0 && less(policy, lhs[0], rhs[0]));
}

/**
 * Vector @p src rotated by pi/2 counter-clockwise.
 */
template <class T>
inline constexpr vec<T> perpendicular(const vec<T>& src)
{
    return vec{-src[1], src[0]};
}

template <class T0, class T1>
inline constexpr auto cross(const vec3<T0>& lhs, const vec3<T1>& rhs)
{
    return vec{
        mul(lhs[1], rhs[2]) - mul(lhs[2], rhs[1]),
        mul(lhs[2], rhs[0]) - mul(lhs[0], rhs[2]),
        mul(lhs[0], rhs[1]) - mul(lhs[1], rhs[0])};
}

} // namespace ac::math
