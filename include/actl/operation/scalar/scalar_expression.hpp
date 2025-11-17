// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/constant.hpp>
#include <actl/operation/expression/expression.hpp>

namespace ac {

template<typename T>
    requires(!is_expression_v<T>)
constexpr T eval(T x) {
    return x;
}

template<typename Expr, size_t... Is>
constexpr decltype(auto) eval_impl(Expr const& e, std::index_sequence<Is...>) {
    return e.operation().evaluate(std::get<Is + 1>(e.args)...);
}

template<typename Expr>
    requires is_expression_v<Expr>
constexpr decltype(auto) eval(Expr const& e) {
    using helper = expression_helper<Expr>;
    if constexpr (helper::is_resolved)
        return eval_impl(e, argument_indices<Expr>{});
    else
        return eval(helper::resolve_expr(e));
}

template<typename T>
inline constexpr bool is_scalar_expression_v =
    std::is_scalar_v<unwrap_constant_t<decltype(eval(std::declval<T>()))>>;

template<typename T>
    requires is_scalar_expression_v<T>
struct can_convert_expression_implicitly<T> : std::true_type {};

} // namespace ac
