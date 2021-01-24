// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

struct logical_and_t : scalar_operation<logical_and_t, 2> {
    using category = logical_operation_tag;
    using argument_category = boolean_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

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

} // namespace ac
