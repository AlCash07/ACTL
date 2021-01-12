// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/scalar_operation.hpp>

namespace ac {

struct bit_and_t : scalar_operation<bit_and_t, 2, integral_tag> {
    struct is_associative;
    struct is_commutative;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs & rhs;
    }
};
constexpr bit_and_t bit_and;

template <class T>
struct neutral_element<bit_and_t, T> {
    static constexpr T value() {
        return T{-1};
    }
};

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator&(T&& lhs, U&& rhs) {
    return bit_and(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator&=(T& lhs, const U& rhs) {
    return bit_and(inout(lhs), rhs);
}

}  // namespace ac
