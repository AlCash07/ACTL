/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/policy.hpp>
#include <actl/numeric/math.hpp>
#include <actl/range/traits.hpp>
#include <algorithm>

namespace ac {

namespace op {

// class E should provide public `T epsilon()`;
template <class E>
struct absolute_error : E, virtual policy {};

DEFINE_HAS_BINARY_OPERATOR(eq, ==)
DEFINE_HAS_OVERLOAD(equal)

template <class T, class U, enable_int_if<!has_equal_v<T, U> && has_eq_v<T, U>> = 0>
inline constexpr auto equal(policy, const T& lhs, const U& rhs) {
    return lhs == rhs;
}

template <class E, class T, class U>
inline constexpr auto equal(const absolute_error<E>& policy, const T& lhs, const U& rhs) {
    return less(default_policy, adl::abs(rhs - lhs), policy.epsilon());
}

template <class Policy, class R0, class R1, enable_int_if<is_range_v<R0> && is_range_v<R1>> = 0>
inline bool equal(Policy&& policy, const R0& lhs, const R1& rhs) {
    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs),
                      [&policy](const auto& x, const auto& y) { return equal(policy, x, y); });
}

template <class T, class U, enable_int_if<has_equal_v<T, U>> = 0>
inline constexpr auto operator == (const T& lhs, const U& rhs) {
    return equal(default_policy, lhs, rhs);
}

template <class T, class U>
inline constexpr auto operator != (const T& lhs, const U& rhs) {
    return !(lhs == rhs);
}

DEFINE_HAS_BINARY_OPERATOR(lt, <)
DEFINE_HAS_OVERLOAD(less)

template <class T, class U, enable_int_if<!has_less_v<T, U> && has_lt_v<T, U>> = 0>
inline constexpr auto less(policy, const T& lhs, const U& rhs) {
    return lhs < rhs;
}

template <class E, class T, class U>
inline constexpr auto less(const absolute_error<E>& policy, const T& lhs, const U& rhs) {
    return less(default_policy, policy.epsilon(), rhs - lhs);
}

template <class Policy, class R0, class R1, enable_int_if<is_range_v<R0> && is_range_v<R1>> = 0>
inline bool less(Policy&& policy, const R0& lhs, const R1& rhs) {
    return std::lexicographical_compare(
        std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs),
        [&policy](const auto& x, const auto& y) { return less(policy, x, y); });
}

template <class T, class U, enable_int_if<has_less_v<T, U>> = 0>
inline constexpr auto operator < (const T& lhs, const U& rhs) {
    return less(default_policy, lhs, rhs);
}

template <class T, class U>
inline constexpr auto operator > (const T& lhs, const U& rhs) {
    return rhs < lhs;
}

template <class T, class U>
inline constexpr auto operator <= (const T& lhs, const U& rhs) {
    return !(lhs > rhs);
}

template <class T, class U>
inline constexpr auto operator >= (const T& lhs, const U& rhs) {
    return !(lhs < rhs);
}

}  // namespace op

template <class Policy, class T, enable_int_if_policy<Policy> = 0>
inline constexpr int sgn(Policy&& policy, const T& x, const T& y = T{0}) {
    return (int)op::less(policy, y, x) - (int)op::less(policy, x, y);
}

template <class T>
inline constexpr int sgn(const T& x, const T& y = T{0}) {
    return sgn(default_policy, x, y);
}

template <class Policy, class T, class U>
inline constexpr T& smax(Policy&& policy, T& x, const U& y) {
    return op::less(policy, x, y) ? x = y : x;
}

template <class T, class U>
inline constexpr T& smax(T& x, const U& y) {
    return smax(default_policy, x, y);
}

template <class Policy, class T, class U>
inline constexpr T& smin(Policy&& policy, T& x, const U& y) {
    return op::less(policy, y, x) ? x = y : x;
}

template <class T, class U>
inline constexpr T& smin(T& x, const U& y) {
    return smin(default_policy, x, y);
}

}  // namespace ac
