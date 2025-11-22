// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

namespace scalar {

struct LogicalNot : operation_base<LogicalNot> {
    using operation_category = logical_operation_tag;

    static constexpr bool evaluate(bool x) {
        return !x;
    }
};
inline constexpr LogicalNot logical_not;

} // namespace scalar

struct LogicalNot : operation_base<LogicalNot> {
    using operation_category = logical_operation_tag;

    static constexpr auto formula = scalar::logical_not;
};
inline constexpr LogicalNot logical_not;

template<typename T>
    requires EnableOperators<T>
constexpr auto operator!(T&& x) {
    return logical_not(pass<T>(x));
}

} // namespace ac
