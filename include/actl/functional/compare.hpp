/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation.hpp>
#include <actl/numeric/math.hpp>
#include <actl/range/traits.hpp>
#include <algorithm>

namespace ac {

namespace op {

struct Sgn : scalar_operation<1, Sgn> {};

struct Equal : scalar_operation<2, Equal> {
    struct is_commutative;
};

struct Less : scalar_operation<2, Less> {};

struct Max : scalar_operation<2, Max> {
    struct is_commutative;
    struct is_associative;
};

struct Min : scalar_operation<2, Min> {
    struct is_commutative;
    struct is_associative;
};

// inline constexpr Sgn sgn;
inline constexpr Equal eq;
inline constexpr Less lt;
inline constexpr Max max;
inline constexpr Min min;

template <class T, class U, enable_int_if<!can_perform_v<Equal, policy, T, U>> = 0>
inline constexpr auto perform(Equal, policy, const T& lhs, const U& rhs) -> decltype(lhs == rhs) {
    return lhs == rhs;
}

template <class... Ts>
inline constexpr auto equal(Ts&&... xs) {
    return eq(std::forward<Ts>(xs)...);
}

template <class Policy>
inline auto equal_functor(Policy& policy) {
    return [&policy](const auto& x, const auto& y) { return equal(policy, x, y); };
}

template <class Policy, class R0, class R1, enable_int_if<is_range_v<R0> && is_range_v<R1>> = 0>
inline bool perform(Equal, const Policy& policy, const R0& lhs, const R1& rhs) {
    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs),
                      equal_functor(policy));
}

template <class T, class U>
inline constexpr auto operator == (const T& lhs, const U& rhs) -> decltype(eq(lhs, rhs)) {
    return eq(lhs, rhs);
}

template <class T, class U>
inline constexpr bool operator != (const T& lhs, const U& rhs) {
    return !(lhs == rhs);
}

template <class T, class U, enable_int_if<!can_perform_v<Less, policy, T, U>> = 0>
inline constexpr auto perform(Less, policy, const T& lhs, const U& rhs) -> decltype(lhs < rhs) {
    return lhs < rhs;
}

template <class T, class U>
inline constexpr auto less(policy, const T& lhs, const U& rhs) {
    return lt(lhs, rhs);
}

template <class Policy, class R0, class R1, enable_int_if<is_range_v<R0> && is_range_v<R1>> = 0>
inline bool perform(Less, const Policy& policy, const R0& lhs, const R1& rhs) {
    return std::lexicographical_compare(std::begin(lhs), std::end(lhs), std::begin(rhs),
                                        std::end(rhs), less_functor(policy));
}

template <class Policy>
inline auto less_functor(Policy& policy) {
    return [&policy](const auto& x, const auto& y) { return less(policy, x, y); };
}

template <class Policy>
inline auto greater_functor(Policy& policy) {
    return [&policy](const auto& x, const auto& y) { return less(policy, y, x); };
}

template <class T, class U>
inline constexpr auto operator < (const T& lhs, const U& rhs) -> decltype(lt(lhs, rhs)) {
    return lt(lhs, rhs);
}

template <class T, class U>
inline constexpr auto operator > (const T& lhs, const U& rhs) -> decltype(rhs < lhs) {
    return rhs < lhs;
}

template <class T, class U>
inline constexpr auto operator <= (const T& lhs, const U& rhs) -> decltype(!(lhs > rhs)) {
    return !(lhs > rhs);
}

template <class T, class U>
inline constexpr auto operator >= (const T& lhs, const U& rhs) -> decltype(!(lhs < rhs)) {
    return !(lhs < rhs);
}

// class E should provide public `T epsilon()`;
template <class E>
struct absolute_error : E, virtual policy {};

template <class E, class T, class U>
inline constexpr auto perform(Equal, const absolute_error<E>& policy, const T& lhs, const U& rhs) {
    return lt(policy, adl::abs(rhs - lhs), policy.epsilon());
}

}  // namespace op

template <class Policy, class T, class U = int, enable_int_if_policy<Policy> = 0>
inline constexpr int sgn(const Policy& policy, const T& x, const U& y = {}) {
    return (int)less(policy, y, x) - (int)less(policy, x, y);
}

template <class E, class T, class U = int>
inline constexpr int sgn(const op::absolute_error<E>& policy, const T& x, const U& y = {}) {
    auto delta = x - y;
    if (adl::abs(delta) < policy.epsilon()) return 0;
    return delta < 0 ? -1 : 1;
}

template <class T, class U = int, disable_int_if_policy<T> = 0>
inline constexpr int sgn(const T& x, const U& y = {}) {
    return sgn(default_policy, x, y);
}

template <class Policy, class T, class U>
inline constexpr T& smax(const Policy& policy, T& x, const U& y) {
    return less(policy, x, y) ? x = y : x;
}

template <class T, class U>
inline constexpr T& smax(T& x, const U& y) {
    return smax(default_policy, x, y);
}

template <class Policy, class T, class U>
inline constexpr T& smin(const Policy& policy, T& x, const U& y) {
    return less(policy, y, x) ? x = y : x;
}

template <class T, class U>
inline constexpr T& smin(T& x, const U& y) {
    return smin(default_policy, x, y);
}

}  // namespace ac
