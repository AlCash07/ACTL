// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/core/operation_expression.hpp>

namespace ac {

template <class Context, class Op, class... Us, class... Ts>
struct is_overload_resolved<
    std::enable_if_t<is_operation_v<expression<Op, Us...>>>,
    Context,
    expression<Op, Us...>,
    Ts...>
{
    static constexpr bool value =
        (... && is_overload_resolved_v<
                    Context,
                    Us,
                    Ts...>)&&is_overload_resolved_v<Context, Op, Us...>;
};

namespace detail {

template <class Context, class Op, class... Ts>
constexpr auto make_resolved_expression(Context context, Op&& op, Ts&&... xs)
{
    return expression{
        resolve_overload_if_can<Ts...>(context, std::forward<Op>(op)),
        std::forward<Ts>(xs)...};
}

template <class... Ts, class Context, class OE, size_t... Is>
constexpr auto resolve_expression(
    Context context, OE&& oe, std::index_sequence<Is...>)
{
    return make_resolved_expression(
        context,
        static_cast<OE&&>(oe).operation(),
        resolve_overload_if_can<Ts...>(
            context, std::get<Is + 1>(static_cast<OE&&>(oe).args))...);
}

} // namespace detail

template <
    class... Ts,
    class Context,
    class OE,
    enable_int_if<
        is_expression_v<OE> && !is_overload_resolved_v<Context, OE, Ts...>> = 0>
constexpr auto resolve_overload(Context context, OE&& oe)
{
    return detail::resolve_expression<Ts...>(
        context, std::forward<OE>(oe), argument_indices<OE>{});
}

} // namespace ac
