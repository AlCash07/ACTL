// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/operation/scalar/scalar_operation.hpp>
#include <actl/utility/none.hpp>

namespace ac {

struct common_t : scalar_operation<common_t, 2> {
    using category = scalar_operation_tag;
    using argument_category = unclassified_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

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
    static constexpr auto eval_scalar(
        std::integral_constant<T, X> x, std::integral_constant<T, X>) //
    {
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

} // namespace ac
