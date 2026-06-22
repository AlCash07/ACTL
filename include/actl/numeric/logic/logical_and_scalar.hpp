// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/logic/logical_operation.hpp>

namespace ac {

struct LogicalAnd;

struct LogicalAndScalar : operation_base<LogicalAndScalar> {
    using parent = LogicalAnd;

    static constexpr bool is_associative = true;
    static constexpr bool is_commutative = true;

    template<std::same_as<bool> T>
    static constexpr auto evaluate(T l, T r) {
        return l && r;
    }
};
inline constexpr LogicalAndScalar logical_and_scalar;

} // namespace ac
