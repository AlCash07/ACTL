// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/logic/logical_operation.hpp>

namespace ac {

struct LogicalAnd : operation_base<LogicalAnd> {
    using operation_category = logical_operation;
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

struct LogicalAndScalar : operation_base<LogicalAndScalar> {
    using parent = LogicalAnd;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr bool requirement =
        std::is_arithmetic_v<L> && std::is_arithmetic_v<R>;

    template<std::same_as<bool> T>
    static constexpr auto evaluate(T l, T r) {
        return l && r;
    }
};
AC_REGISTER_OVERLOAD(LogicalAndScalar)
inline constexpr LogicalAndScalar logical_and_scalar;

} // namespace ac
