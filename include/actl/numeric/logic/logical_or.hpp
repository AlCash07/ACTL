// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/logic/logical_operation.hpp>

namespace ac {

struct LogicalOr : operation_base<LogicalOr> {
    using operation_category = logical_operation;
};
inline constexpr LogicalOr logical_or;

template<typename T>
struct identity_element<LogicalOr, T> {
    static constexpr T value() {
        return T{false};
    }
};

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator||(L&& l, R&& r) {
    return logical_or(std::forward<L>(l), std::forward<R>(r));
}

struct LogicalOrScalar : operation_base<LogicalOrScalar> {
    using parent = LogicalOr;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<typename L, typename R>
    static constexpr bool requirement =
        std::is_arithmetic_v<L> && std::is_arithmetic_v<R>;

    template<std::same_as<bool> T>
    static constexpr auto evaluate(T l, T r) {
        return l || r;
    }
};
AC_REGISTER_OVERLOAD(LogicalOrScalar)
inline constexpr LogicalOrScalar logical_or_scalar;

} // namespace ac
