// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/assert.hpp>
#include <actl/functional/scalar/scalar_operation.hpp>
#include <actl/utility/none.hpp>

namespace ac {

constexpr std::integral_constant<int, 0> zero;
constexpr std::integral_constant<int, 1> one;

template <index I, index N>
struct arg_t {
    using category = operation_tag;
    struct enable_operators;

    template <class T, class... Ts>
    constexpr decltype(auto) operator()(T&& x, Ts&&... xs) const {
        static_assert(1 + sizeof...(Ts) == N || N == -1);
        if constexpr (I == 0)
            return std::forward<T>(x);
        else
            return arg_t<I - 1, (N == -1 ? -1 : N - 1)>{}(std::forward<Ts>(xs)...);
    }
};
template <index I, index N>
constexpr arg_t<I, N> arg;

constexpr arg_t<0, 1> x_;
constexpr arg_t<0, 2> lhs_;
constexpr arg_t<1, 2> rhs_;

template <class To>
struct cast_t : scalar_operation<cast_t<To>, 1, arithmetic_tag> {
    template <class T>
    static constexpr To eval_scalar(T x) {
        return static_cast<To>(x);
    }
};
template <class T>
constexpr cast_t<T> cast;

struct common_t : scalar_operation<common_t, 2, unclassified_tag> {
    struct is_associative;
    struct is_commutative;

    template <class... Ts>
    constexpr auto evaluate(const Ts&... xs) const {
        return eval_scalar(eval(xs)...);
    }

    template <class T>
    static constexpr T eval_scalar(T x) {
        return x;
    }

    template <class T>
    static T eval_scalar(T x, T y) {
        ACTL_ASSERT(x == y);
        return x;
    }

    static constexpr auto eval_scalar(none, none) {
        return none{};
    }

    template <class T>
    static constexpr T eval_scalar(T x, none) {
        return x;
    }

    template <class U>
    static constexpr U eval_scalar(none, U y) {
        return y;
    }

    template <class T, T X>
    static constexpr auto eval_scalar(std::integral_constant<T, X> x,
                                      std::integral_constant<T, X>) {
        return x;
    }

    template <class T, T X, class U, enable_int_if<std::is_integral_v<U>> = 0>
    static auto eval_scalar(std::integral_constant<T, X> x, U y) {
        ACTL_ASSERT(X == y);
        return x;
    }

    template <class T, class U, U Y, enable_int_if<std::is_integral_v<T>> = 0>
    static auto eval_scalar(T x, std::integral_constant<U, Y> y) {
        return eval_scalar(y, x);
    }

    template <class T0, class T1, class T2, class... Ts>
    static constexpr auto eval_scalar(T0 x0, T1 x1, T2 x2, Ts... xs) {
        return eval_scalar(eval_scalar(x0, x1), x2, xs...);
    }
};
constexpr common_t common;

template <class T>
struct neutral_element<common_t, T> {
    static constexpr none value() {
        return none{};
    }
};

struct copy_t : scalar_operation<copy_t, 1, arithmetic_tag> {
    template <class T>
    static constexpr T eval_scalar(T x) {
        return x;
    }
};
constexpr copy_t copy;

template <class T, class U>
constexpr void assign(out_t<false, T>& dst, const U& y) {
    copy(dst, y);
}

struct select_t : scalar_operation<select_t, 3, arithmetic_tag> {
    template <class T, class U>
    constexpr auto evaluate(bool condition, const T& lhs, const U& rhs) const {
        return condition ? eval(lhs) : eval(rhs);
    }
};
constexpr select_t select;

}  // namespace ac
