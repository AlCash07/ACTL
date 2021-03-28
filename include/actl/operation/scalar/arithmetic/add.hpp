// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct add_f : scalar_operation<add_f, 2> {
    using category = additive_operation_tag;
    using argument_category = arithmetic_tag;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs + rhs;
    }
};
inline constexpr add_f add;

} // namespace scalar

struct add_f : operation<add_f> {
    using category = additive_operation_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    static constexpr auto formula = scalar::add;
};
inline constexpr add_f add;

template <class T>
struct neutral_element<add_f, T> {
    static constexpr T value() {
        return T{0};
    }
};

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator+(T&& lhs, U&& rhs) {
    return add(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator+=(T&& lhs, U&& rhs) {
    return add(inout{std::forward<T>(lhs)}, pass<U>(rhs));
}

} // namespace ac
