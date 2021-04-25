// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/core/expression.hpp>

namespace ac {

template <class... Ts>
struct expression_base<expression<Ts...>, scalar_tag> {
    struct type {
        constexpr operator resolved_result_type_t<Ts...>() const {
            return eval(static_cast<const expression<Ts...>&>(*this));
        }
    };
};

template <class T, enable_int_if<!is_expression_v<T>> = 0>
constexpr T eval(T x) {
    return x;
}

template <class Expr, size_t... Is>
constexpr decltype(auto) eval_impl(const Expr& e, std::index_sequence<Is...>) {
    return e.operation().evaluate(std::get<Is + 1>(e.args)...);
}

template <class Expr, enable_int_if<is_expression_v<Expr>> = 0>
constexpr decltype(auto) eval(const Expr& e) {
    using helper = expression_helper<Expr>;
    if constexpr (helper::is_resolved)
        return eval_impl(e, argument_indices<Expr>{});
    else
        return eval(helper::resolve_expr(e));
}

} // namespace ac
