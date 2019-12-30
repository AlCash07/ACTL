/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation.hpp>

namespace ac {

namespace op {

struct Neg : scalar_operation<1, Neg> {};

struct Sqr : scalar_operation<1, Sqr> {};

struct Add : scalar_operation<2, Add> {
    struct is_commutative;
    struct is_associative;
};

struct Div : scalar_operation<2, Div> {};

struct Mul : scalar_operation<2, Mul> {
    struct is_commutative;
    struct is_associative;
};

struct Sub : scalar_operation<2, Sub> {};

inline constexpr Neg neg;
inline constexpr Sqr sqr;

inline constexpr Add add;
inline constexpr Div div;
inline constexpr Mul mul;
inline constexpr Sub sub;

template <class T, enable_int_if<!can_perform_v<Neg, policy, T>> = 0>
inline constexpr auto perform(Neg, policy, const T& x) -> decltype(-x) {
    return -x;
}

template <class T, enable_int_if<!can_perform_v<Sqr, policy, T>> = 0>
inline constexpr auto perform(Sqr, policy, const T& x) -> decltype(x * x) {
    return x * x;
}

template <class T, class U, enable_int_if<!can_perform_v<Div, policy, T, U>> = 0>
inline constexpr auto perform(Div, policy, const T& lhs, const U& rhs) -> decltype(lhs / rhs) {
    return lhs / rhs;
}

template <class... Ts>
inline constexpr auto ratio(Ts&&... xs) {
    return div(std::forward<Ts>(xs)...);
}

template <class T, class U>
inline constexpr auto operator / (const T& lhs, const U& rhs) -> decltype(div(lhs, rhs)) {
    return div(lhs, rhs);
}

template <class T, class U, enable_int_if<!can_perform_v<Mul, policy, T, U>> = 0>
inline constexpr auto perform(Mul, policy, const T& lhs, const U& rhs) -> decltype(lhs * rhs) {
    return lhs * rhs;
}

template <class... Ts>
inline constexpr auto product(Ts&&... xs) {
    return perform(mul, std::forward<Ts>(xs)...);
}

template <class T, class U>
inline constexpr auto operator * (const T& lhs, const U& rhs) -> decltype(mul(lhs, rhs)) {
    return mul(lhs, rhs);
}

template <class Op, class T>
struct cast_before : virtual policy {};

template <class Op, class T, class... Ts>
inline constexpr auto perform(Op op, cast_before<Op, T>, const Ts&... xs) 
    -> decltype(perform(op, default_policy, static_cast<T>(xs)...)) {
    return perform(op, default_policy, static_cast<T>(xs)...);
}

}  // namespace op

template <class Policy, class T>
inline constexpr auto sqr(const Policy& policy, const T& x) {
    return product(policy, x, x);
}

template <class T>
inline constexpr auto sqr(const T& x) {
    return x * x;
}

}  // namespace ac
