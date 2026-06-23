// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/logic/logical_or.hpp>

namespace ac {

struct LogicalOrScalar : operation_base<LogicalOrScalar> {
    using parent = LogicalOr::op;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<std::same_as<bool> T>
    static constexpr auto evaluate(T l, T r) {
        return l || r;
    }
};
inline constexpr LogicalOrScalar logical_or_scalar;

namespace LogicalOr {
template<typename L, typename R>
    requires(std::is_arithmetic_v<L> && std::is_arithmetic_v<R>)
constexpr auto specialization(op, type_array<L, R>) noexcept {
    return logical_or_scalar;
}
} // namespace LogicalOr

} // namespace ac
