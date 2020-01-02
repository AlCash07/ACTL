/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation.hpp>

namespace ac::op {

struct Neg : scalar_operation<1, Neg> {
    template <class T>
    static constexpr auto fallback(const T& x) -> decltype(-x) {
        return -x;
    }
};

struct Sqr : scalar_operation<1, Sqr> {};

struct Add : scalar_operation<2, Add> {
    struct is_commutative;
    struct is_associative;

    template <class T, class U>
    static constexpr auto fallback(const T& x, const U& y) -> decltype(x + y) {
        return x + y;
    }
};

struct Div : scalar_operation<2, Div> {
    template <class T, class U>
    static constexpr auto fallback(const T& x, const U& y) -> decltype(x / y) {
        return x / y;
    }
};

struct Mul : scalar_operation<2, Mul> {
    struct is_commutative;
    struct is_associative;

    template <class T, class U>
    static constexpr auto fallback(const T& x, const U& y) -> decltype(x * y) {
        return x * y;
    }
};

struct Sub : scalar_operation<2, Sub> {
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
    return div(lhs, rhs);
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
