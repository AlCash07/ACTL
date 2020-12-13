// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/scalar_operation.hpp>

namespace ac::math {

struct BitNot : scalar_operation<BitNot, 2, integral_tag> {
    template <class T>
    static constexpr T eval_scalar(T x) {
        return ~x;
    }
};
inline constexpr BitNot bit_not;

template <class T>
inline constexpr auto operator ~ (T&& x) {
    return bit_not(pass<T>(x));
}

struct BitAnd : scalar_operation<BitAnd, 2, integral_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs & rhs;
    }
};
inline constexpr BitAnd bit_and;

template <class T, class U>
inline constexpr auto operator & (T&& lhs, U&& rhs) {
    return bit_and(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U>
inline constexpr decltype(auto) operator &= (T& lhs, const U& rhs) {
    return bit_and(inplace(lhs), rhs);
}

struct BitOr : scalar_operation<BitOr, 2, integral_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs | rhs;
    }
};
inline constexpr BitOr bit_or;

template <class T, class U>
inline constexpr auto operator | (T&& lhs, U&& rhs) {
    return bit_or(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U>
inline constexpr decltype(auto) operator |= (T& lhs, const U& rhs) {
    return bit_or(inplace(lhs), rhs);
}

struct BitXor : scalar_operation<BitXor, 2, integral_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs ^ rhs;
    }
};
inline constexpr BitXor bit_xor;

template <class T, class U>
inline constexpr auto operator ^ (T&& lhs, U&& rhs) {
    return bit_xor(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U>
inline constexpr decltype(auto) operator ^= (T& lhs, const U& rhs) {
    return bit_xor(inplace(lhs), rhs);
}

}  // namespace ac::math
