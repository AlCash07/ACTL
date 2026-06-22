// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/logic/logical_and_scalar.hpp>

namespace ac {

struct LogicalAnd : operation_base<LogicalAnd> {
    using operation_category = logical_operation;

    static constexpr auto identity_element = constant<true>{};

    template<typename L, typename R>
        requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
    friend constexpr auto specialization(
        LogicalAnd, type_array<L, R>
    ) noexcept {
        return logical_and_scalar;
    }
};
inline constexpr LogicalAnd logical_and;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator&&(L&& l, R&& r) {
    return logical_and(std::forward<L>(l), std::forward<R>(r));
}

} // namespace ac
