/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/numeric/math.hpp>
#include <actl/util/use_default.hpp>

namespace ac {

// class E should provide public `T epsilon()`;
template <class E>
struct absolute_epsilon : E {
    struct is_policy;
};

// We can't use name `equal` because it's defined in namespace std.
template <class T>
inline constexpr bool eq(use_default, const T& lhs, const T& rhs) {
    return lhs == rhs;
}

template <class E, class T>
inline constexpr bool eq(const absolute_epsilon<E>& policy, const T& lhs, const T& rhs) {
    return adl::abs(rhs - lhs) < policy.epsilon();
}

template <class T>
inline constexpr bool eq(const T& lhs, const T& rhs) {
    return eq(use_default{}, lhs, rhs);
}

// We can't use name `less` because it's defined in namespace std.
template <class T>
inline constexpr bool lt(use_default, const T& lhs, const T& rhs) {
    return lhs < rhs;
}

template <class E, class T>
inline constexpr bool lt(const absolute_epsilon<E>& policy, const T& lhs, const T& rhs) {
    return policy.epsilon() < rhs - lhs;
}

template <class T>
inline constexpr bool lt(const T& lhs, const T& rhs) {
    return lt(use_default{}, lhs, rhs);
}

template <class Policy, class T>
inline constexpr int sgn(const Policy& policy, const T& x, const T& y = T{0}) {
    return lt(policy, y, x) - lt(policy, x, y);
}

template <class T>
inline constexpr int sgn(const T& x, const T& y = T{0}) {
    return sgn(use_default{}, x, y);
}

template <class Policy, class T>
inline constexpr T& smax(const Policy& policy, T& x, const T& y) {
    return lt(policy, x, y) ? x = y : x;
}

template <class T>
inline constexpr T& smax(T& x, const T& y) {
    return smax(use_default{}, x, y);
}

template <class Policy, class T>
inline constexpr T& smin(const Policy& policy, T& x, const T& y) {
    return lt(policy, y, x) ? x = y : x;
}

template <class T>
inline constexpr T& smin(T& x, const T& y) {
    return smin(use_default{}, x, y);
}

}  // namespace ac
