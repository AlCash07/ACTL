// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/logic/logical_or_scalar.hpp>

namespace ac {

struct LogicalOr : operation_base<LogicalOr> {
    using operation_category = logical_operation;

    static constexpr auto identity_element = constant<false>{};

    template<typename L, typename R>
        requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
    friend constexpr auto specialization(LogicalOr, type_array<L, R>) noexcept {
        return logical_or_scalar;
    }
};
inline constexpr LogicalOr logical_or;

template<typename L, typename R>
    requires EnableOperators<L, R>
constexpr auto operator||(L&& l, R&& r) {
    return logical_or(std::forward<L>(l), std::forward<R>(r));
}

} // namespace ac
