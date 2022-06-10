// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/core/expression.hpp>

namespace ac {

template <class T>
struct can_convert_expression_implicitly<T, std::enable_if_t<is_scalar_v<T>>>
    : std::true_type
{};

template <class T, enable_int_if<!is_expression_v<T>> = 0>
constexpr T eval(T x)
{
    return x;
}

template <class Expr, size_t... Is>
constexpr decltype(auto) eval_impl(Expr const& e, std::index_sequence<Is...>)
{
    return e.operation().evaluate(std::get<Is + 1>(e.args)...);
}

template <class Expr, enable_int_if<is_expression_v<Expr>> = 0>
constexpr decltype(auto) eval(Expr const& e)
{
    using helper = expression_helper<Expr>;
    if constexpr (helper::is_resolved)
        return eval_impl(e, argument_indices<Expr>{});
    else
        return eval(helper::resolve_expr(e));
}

} // namespace ac
