// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/logic/logical_not.hpp>

namespace ac {

struct LogicalNotScalar : operation_base<LogicalNotScalar> {
    using parent = LogicalNot::op;

    static constexpr auto evaluate(std::same_as<bool> auto b) {
        return !b;
    }
};
inline constexpr LogicalNotScalar logical_not_scalar;

namespace LogicalNot {
template<typename T>
    requires std::is_arithmetic_v<T>
constexpr auto specialization(op, type_array<T>) noexcept {
    return logical_not_scalar;
}
} // namespace LogicalNot

} // namespace ac
