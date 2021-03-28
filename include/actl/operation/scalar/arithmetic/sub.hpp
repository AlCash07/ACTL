// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct sub_f : scalar_operation<sub_f, 2> {
    using category = additive_operation_tag;
    using argument_category = arithmetic_tag;

    template <class T, class U>
    static constexpr auto eval_scalar(T lhs, U rhs) {
        return lhs - rhs;
    }
};
inline constexpr sub_f sub;

} // namespace scalar

struct sub_f : operation<sub_f> {
    using category = additive_operation_tag;

    static constexpr auto formula = scalar::sub;
};
inline constexpr sub_f sub;

template <class T, class U, enable_operators<T, U> = 0>
constexpr auto operator-(T&& lhs, U&& rhs) {
    return sub(pass<T>(lhs), pass<U>(rhs));
}

template <class T, class U, enable_operators<T, U> = 0>
constexpr decltype(auto) operator-=(T&& lhs, U&& rhs) {
    return sub(inout{std::forward<T>(lhs)}, pass<U>(rhs));
}

} // namespace ac
