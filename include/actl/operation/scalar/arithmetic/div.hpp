// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct div_f : scalar_operation<div_f, 2> {
    using operation_category = multiplicative_operation_tag;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs / rhs;
    }
};
inline constexpr div_f div;

} // namespace scalar

struct div_f : operation<div_f> {
    using operation_category = multiplicative_operation_tag;

    static constexpr auto formula = scalar::div;
};
inline constexpr div_f div;

template <class T, class U>
    requires EnableOperators<T, U>
constexpr auto operator/(T&& lhs, U&& rhs) {
    return div(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U>
    requires EnableOperators<T, U>
constexpr decltype(auto) operator/=(T&& lhs, U&& rhs) {
    return div(inout{std::forward<T>(lhs)}, pass<U>(rhs));
}

} // namespace ac
