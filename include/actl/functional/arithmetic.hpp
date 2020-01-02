/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation.hpp>

namespace ac::op {

struct arithmetic_operation_tag : scalar_operation_tag {};

// Base class for scalar operations.
template <class Derived, int Arity>
struct arithmetic_operation : scalar_operation<Derived, Arity> {
    using operation_tag = arithmetic_operation_tag;
};

struct Neg : arithmetic_operation<Neg, 1> {
    template <class T>
    static constexpr auto fallback(const T& x) -> decltype(-x) {
        return -x;
    }
};

struct Sqr : arithmetic_operation<Sqr, 1> {};

struct Add : arithmetic_operation<Add, 2> {
    struct is_commutative;
    struct is_associative;

    template <class T, class U>
    static constexpr auto fallback(const T& x, const U& y) -> decltype(x + y) {
        return x + y;
    }
};

struct Div : arithmetic_operation<Div, 2> {
    template <class T, class U>
    static constexpr auto fallback(const T& x, const U& y) -> decltype(x / y) {
        return x / y;
    }
};

struct Mul : arithmetic_operation<Mul, 2> {
    struct is_commutative;
    struct is_associative;

    template <class T, class U>
    static constexpr auto fallback(const T& x, const U& y) -> decltype(x * y) {
        return x * y;
    }
};

struct Sub : arithmetic_operation<Sub, 2> {
    template <class T, class U>
    static constexpr auto fallback(const T& x, const U& y) -> decltype(x - y) {
        return x - y;
    }
};

inline constexpr Neg neg;
inline constexpr Sqr sqr;

inline constexpr Add add;
inline constexpr Div div;
inline constexpr Mul mul;
inline constexpr Sub sub;

template <class Policy, class T>
inline constexpr auto perform(Sqr, const Policy& policy, const T& x) {
    return mul(policy, x, x);
}

template <class T>
inline constexpr auto operator - (const T& x) -> decltype(neg(x)) {
    return neg(x);
}

template <class T, class U>
inline constexpr auto operator + (const T& lhs, const U& rhs) -> decltype(add(lhs, rhs)) {
    return add(lhs, rhs);
}

template <class T, class U>
inline constexpr auto operator / (const T& lhs, const U& rhs) -> decltype(div(lhs, rhs)) {
    return div(lhs, rhs);
}

template <class T, class U>
inline constexpr auto operator * (const T& lhs, const U& rhs) -> decltype(mul(lhs, rhs)) {
    return mul(lhs, rhs);
}

template <class T, class U>
inline constexpr auto operator - (const T& lhs, const U& rhs) -> decltype(sub(lhs, rhs)) {
    return sub(lhs, rhs);
}

}  // namespace ac::op
