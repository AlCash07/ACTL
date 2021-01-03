// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/scalar_operation.hpp>

namespace ac {

struct bit_not_t : scalar_operation<bit_not_t, 2, integral_tag> {
    template <class T>
    static constexpr T eval_scalar(T x) {
        return ~x;
    }
};
constexpr bit_not_t bit_not;

template <class T, enable_operators<T> = 0>
constexpr auto operator~(T&& x) {
    return bit_not(pass<T>(x));
}

struct bit_and_t : scalar_operation<bit_and_t, 2, integral_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs & rhs;
    }
};
constexpr bit_and_t bit_and;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator&(T&& lhs, U&& rhs) {
    return bit_and(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator&=(T& lhs, const U& rhs) {
    return bit_and(inplace(lhs), rhs);
}

struct bit_or_t : scalar_operation<bit_or_t, 2, integral_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs | rhs;
    }
};
constexpr bit_or_t bit_or;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator|(T&& lhs, U&& rhs) {
    return bit_or(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator|=(T& lhs, const U& rhs) {
    return bit_or(inplace(lhs), rhs);
}

struct bit_xor_t : scalar_operation<bit_xor_t, 2, integral_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs ^ rhs;
    }
};
constexpr bit_xor_t bit_xor;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator^(T&& lhs, U&& rhs) {
    return bit_xor(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator^=(T& lhs, const U& rhs) {
    return bit_xor(inplace(lhs), rhs);
}

}  // namespace ac
