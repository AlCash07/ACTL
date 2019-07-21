/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/policy.hpp>
#include <actl/numeric/math.hpp>

namespace ac {

namespace op {

// class E should provide public `T epsilon()`;
template <class E>
struct absolute_error : E, virtual policy {};

DEFINE_HAS_BINARY_OPERATOR(eq, ==)

template <class T, class U, enable_int_if<has_eq_v<T, U>> = 0>
inline constexpr auto equal(policy, const T& lhs, const U& rhs) {
    return lhs == rhs;
}

template <class E, class T, class U>
inline constexpr auto equal(const absolute_error<E>& policy, const T& lhs, const U& rhs) {
    return less(default_policy, adl::abs(rhs - lhs), policy.epsilon());
}

DEFINE_HAS_BINARY_OPERATOR(lt, <)

template <class T, class U, enable_int_if<has_lt_v<T, U>> = 0>
inline constexpr auto less(policy, const T& lhs, const U& rhs) {
    return lhs < rhs;
}

template <class E, class T, class U>
inline constexpr auto less(const absolute_error<E>& policy, const T& lhs, const U& rhs) {
    return less(default_policy, policy.epsilon(), rhs - lhs);
}

}  // namespace op

template <class Policy, class T>
inline constexpr int sgn(const Policy& policy, const T& x, const T& y = T{0}) {
    return (int)op::less(policy, y, x) - (int)op::less(policy, x, y);
}

template <class T>
inline constexpr int sgn(const T& x, const T& y = T{0}) {
    return sgn(default_policy, x, y);
}

template <class Policy, class T, class U>
inline constexpr T& smax(const Policy& policy, T& x, const U& y) {
    return op::less(policy, x, y) ? x = y : x;
}

template <class T, class U>
inline constexpr T& smax(T& x, const U& y) {
    return smax(default_policy, x, y);
}

template <class Policy, class T, class U>
inline constexpr T& smin(const Policy& policy, T& x, const U& y) {
    return op::less(policy, y, x) ? x = y : x;
}

template <class T, class U>
inline constexpr T& smin(T& x, const U& y) {
    return smin(default_policy, x, y);
}

}  // namespace ac
