// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/logic/logical_operation.hpp>

namespace ac {

struct LogicalNot : operation_base<LogicalNot> {
    using operation_category = logical_operation;
};
inline constexpr LogicalNot logical_not;

template<typename T>
    requires EnableOperators<T>
constexpr auto operator!(T&& t) {
    return logical_not(std::forward<T>(t));
}

struct LogicalNotScalar : operation_base<LogicalNotScalar> {
    using parent = LogicalNot;

    template<typename T>
    static constexpr bool match = std::is_arithmetic_v<T>;

    static constexpr auto evaluate(std::same_as<bool> auto b) {
        return !b;
    }
};
AC_REGISTER_SPECIALIZATION(LogicalNotScalar)
inline constexpr LogicalNotScalar logical_not_scalar;

} // namespace ac
