// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct LogicalAnd : operation_base<LogicalAnd> {
    using operation_category = logical_operation_tag;

    static constexpr bool evaluate(bool l, bool r) {
        return l && r;
    }
};
inline constexpr LogicalAnd logical_and;

} // namespace scalar

struct LogicalAnd : operation_base<LogicalAnd> {
    using operation_category = logical_operation_tag;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    static constexpr auto formula = scalar::logical_and;
};
inline constexpr LogicalAnd logical_and;

template<typename T>
struct identity_element<LogicalAnd, T> {
    static constexpr T value() {
        return T{true};
    }
};

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator&&(L&& l, R&& r) {
    return logical_and(pass<L>(l), pass<R>(r));
}

} // namespace ac
