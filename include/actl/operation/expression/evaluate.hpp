// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/parameter/out.hpp>
#include <actl/operation/expression/expression_data.hpp>
#include <actl/operation/overload/resolve_overload.hpp>

namespace ac {

template<typename T>
    requires(!is_expression_v<T>)
constexpr T eval(T x) {
    return x;
}

template<Operation Op, size_t... Is, typename... Args>
constexpr decltype(auto) eval(
    expression_data<Op, std::index_sequence<Is...>, Args...> const& expression
) {
    auto&& operation =
        resolve_overload<Args...>(default_context{}, expression.operation);
    return operation.evaluate(std::get<Is>(expression.arguments)...);
}

template<typename Target, Operation Op, size_t... Is, typename... Args>
constexpr void assign(
    out<Target>& target,
    expression_data<Op, std::index_sequence<Is...>, Args...> const& expression
) {
    auto&& operation =
        resolve_overload<Args...>(default_context{}, expression.operation);
    operation.evaluate_to(target, std::get<Is>(expression.arguments)...);
}

} // namespace ac
