// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/expression/expression_data.hpp>
#include <actl/operation/type_operation.hpp>

namespace ac {

template<typename T>
    requires(!is_expression_v<T>)
constexpr decltype(auto) strip_placeholders(T&& t) {
    return std::forward<T>(t);
}

template<typename T, typename Arg>
constexpr auto strip_placeholders(Placeholder<T, Arg>) {
    return Arg{};
}

template<Operation Op, size_t... Is, typename... Args>
constexpr auto strip_placeholders(
    const expression_data<Op, std::index_sequence<Is...>, Args...>& expression
) {
    return expression.operation(
        strip_placeholders(expression.arguments[constant<Is>{}])...
    );
}

} // namespace ac
