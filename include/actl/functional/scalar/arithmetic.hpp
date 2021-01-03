// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/scalar_operation.hpp>

namespace ac {

struct neg_t : scalar_operation<neg_t, 2, arithmetic_tag> {
    template <class T>
    static constexpr auto eval_scalar(T x) {
        return -x;
    }
};
constexpr neg_t neg;

template <class T, enable_operators<T> = 0>
constexpr auto operator-(T&& x) {
    return neg(pass<T>(x));
}

struct add_t : scalar_operation<add_t, 2, arithmetic_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs + rhs;
    }
};
constexpr add_t add;

template <class T>
struct neutral_element<add_t, T> {
    static constexpr T value() {
        return T{0};
    }
};

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator+(T&& lhs, U&& rhs) {
    return add(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator+=(T& lhs, const U& rhs) {
    return add(inplace(lhs), rhs);
}

struct sub_t : scalar_operation<sub_t, 2, arithmetic_tag> {
    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs - rhs;
    }
};
constexpr sub_t sub;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator-(T&& lhs, U&& rhs) {
    return sub(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator-=(T& lhs, const U& rhs) {
    return sub(inplace(lhs), rhs);
}

struct mul_t : scalar_operation<mul_t, 2, arithmetic_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs * rhs;
    }
};
constexpr mul_t mul;

template <class T>
struct neutral_element<mul_t, T> {
    static constexpr T value() {
        return T{1};
    }
};

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator*(T&& lhs, U&& rhs) {
    return mul(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator*=(T& lhs, const U& rhs) {
    return mul(inplace(lhs), rhs);
}

struct div_t : scalar_operation<div_t, 2, arithmetic_tag> {
    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs / rhs;
    }
};
constexpr div_t div;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator/(T&& lhs, U&& rhs) {
    return div(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator/=(T& lhs, const U& rhs) {
    return div(inplace(lhs), rhs);
}

}  // namespace ac
