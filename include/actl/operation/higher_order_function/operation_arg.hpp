// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/higher_order_function/strip_placeholders.hpp>
#include <actl/operation/operation/operation_base.hpp>

namespace ac {

struct OperationArg : operation_base<OperationArg> {};
inline constexpr OperationArg operation_arg;

template<typename Arg>
constexpr auto eval(
    const expression_data<OperationArg, std::index_sequence<0>, Arg>& expression
) {
    return strip_placeholders(expression.arguments[constant<0>{}]);
}

} // namespace ac
