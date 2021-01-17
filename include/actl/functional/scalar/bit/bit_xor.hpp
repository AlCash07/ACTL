// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/scalar_operation.hpp>

namespace ac {

struct bit_xor_t : scalar_operation<bit_xor_t, 2, integral_tag> {
    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs ^ rhs;
    }
};
constexpr bit_xor_t bit_xor;

template <class T>
struct neutral_element<bit_xor_t, T> {
    static constexpr T value() {
        return T{0};
    }
};

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator^(T&& lhs, U&& rhs) {
    return bit_xor(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator^=(T&& lhs, U&& rhs) {
    return bit_xor(inout(std::forward<T>(lhs)), pass<U>(rhs));
}

}  // namespace ac