// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/common.hpp>

namespace ac {

struct logical_not_t : scalar_operation<logical_not_t, 1, boolean_tag> {
    static constexpr bool eval_scalar(bool x) {
        return !x;
    }
};
constexpr logical_not_t logical_not;

template <class T, enable_operators<T> = 0>
constexpr auto operator!(T&& x) {
    return logical_not(pass<T>(x));
}

struct logical_and_t : scalar_operation<logical_and_t, 2, boolean_tag> {
    struct is_associative;
    struct is_commutative;

    static constexpr bool eval_scalar(bool lhs, bool rhs) {
        return lhs && rhs;
    }
};
constexpr logical_and_t logical_and;

template <class T>
struct neutral_element<logical_and_t, T> {
    static constexpr T value() {
        return T{true};
    }
};

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator&&(T&& lhs, U&& rhs) {
    return logical_and(pass<T>(lhs), pass<U>(rhs));
}

struct logical_or_t : scalar_operation<logical_or_t, 2, boolean_tag> {
    struct is_associative;
    struct is_commutative;

    static constexpr bool eval_scalar(bool lhs, bool rhs) {
        return lhs || rhs;
    }
};
constexpr logical_or_t logical_or;

template <class T>
struct neutral_element<logical_or_t, T> {
    static constexpr T value() {
        return T{false};
    }
};

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator||(T&& lhs, U&& rhs) {
    return logical_or(pass<T>(lhs), pass<U>(rhs));
}

constexpr auto logical_implies = !lhs_ || rhs_;

}  // namespace ac
