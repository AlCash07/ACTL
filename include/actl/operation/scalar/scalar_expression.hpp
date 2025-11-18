// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/constant.hpp>
#include <actl/operation/expression/operation_expression.hpp>

namespace ac {

template<typename T>
    requires(!is_expression_v<T>)
constexpr T eval(T x) {
    return x;
}

template<typename Operation, size_t... Is, typename... Args>
constexpr decltype(auto) eval(
    expression_storage<Operation, std::index_sequence<Is...>, Args...> const&
        expr
) {
    auto&& op = resolve_overload<Args...>(default_context{}, expr.operation);
    return op.evaluate(std::get<Is>(expr.arguments)...);
}

template<typename T>
inline constexpr bool is_scalar_expression_v =
    std::is_scalar_v<unwrap_constant_t<decltype(eval(std::declval<T>()))>>;

template<typename T>
    requires is_scalar_expression_v<T>
struct can_convert_expression_implicitly<T> : std::true_type {};

} // namespace ac
