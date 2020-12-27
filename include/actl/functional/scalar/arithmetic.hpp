// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/scalar_operation.hpp>

namespace ac::math {

struct Neg : scalar_operation<Neg, 2, arithmetic_tag> {
    template <class T>
    static constexpr auto eval_scalar(T x) {
        return -x;
    }
};
constexpr Neg neg;

template <class T, enable_operators<T> = 0>
constexpr auto operator-(T&& x) {
    return neg(pass<T>(x));
}

struct Add : scalar_operation<Add, 2, arithmetic_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs + rhs;
    }
};
constexpr Add add;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator+(T&& lhs, U&& rhs) {
    return add(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator+=(T& lhs, const U& rhs) {
    return add(inplace(lhs), rhs);
}

struct Sub : scalar_operation<Sub, 2, arithmetic_tag> {
    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs - rhs;
    }
};
constexpr Sub sub;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator-(T&& lhs, U&& rhs) {
    return sub(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator-=(T& lhs, const U& rhs) {
    return sub(inplace(lhs), rhs);
}

struct Mul : scalar_operation<Mul, 2, arithmetic_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs * rhs;
    }
};
constexpr Mul mul;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator*(T&& lhs, U&& rhs) {
    return mul(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator*=(T& lhs, const U& rhs) {
    return mul(inplace(lhs), rhs);
}

struct Div : scalar_operation<Div, 2, arithmetic_tag> {
    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs / rhs;
    }
};
constexpr Div div;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator/(T&& lhs, U&& rhs) {
    return div(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator/=(T& lhs, const U& rhs) {
    return div(inplace(lhs), rhs);
}

}  // namespace ac::math
