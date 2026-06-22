// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/logic/logical_not_scalar.hpp>

namespace ac {

struct LogicalNot : operation_base<LogicalNot> {
    using operation_category = logical_operation;

    template<typename T>
        requires std::is_arithmetic_v<T>
    friend constexpr auto specialization(LogicalNot, type_array<T>) noexcept {
        return logical_not_scalar;
    }
};
inline constexpr LogicalNot logical_not;

template<typename T>
    requires EnableOperators<T>
constexpr auto operator!(T&& t) {
    return logical_not(std::forward<T>(t));
}

} // namespace ac
