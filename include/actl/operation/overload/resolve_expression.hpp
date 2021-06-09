// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/core/operation_expression.hpp>

namespace ac {

namespace detail {

template <class S, class Context, class Op, class... Ts>
struct expression_overload;

template <size_t... Is, class Context, class Op, class... Ts>
struct expression_overload<std::index_sequence<Is...>, Context, Op, Ts...>
{
    template <class Op1, class... Us>
    static constexpr auto make_expression(Context context, Op1&& op, Us&&... xs)
    {
        return expression{
            resolve_overload<Us...>(context, std::forward<Op1>(op)),
            std::forward<Us>(xs)...};
    }

    template <class OE>
    static constexpr auto resolve(Context context, OE&& oe)
    {
        return make_expression(
            context,
            static_cast<OE&&>(oe).operation(),
            resolve_overload<Ts...>(
                context, std::get<Is + 1>(static_cast<OE&&>(oe).args))...);
    }
};

} // namespace detail

template <class Context, class Op, class... Us, class... Ts>
struct overload_resolver<
    std::enable_if_t<
        is_operation_v<expression<Op, Us...>> &&
        !(is_overload_resolved_v<Context, Op, Us...> &&
          (... && is_overload_resolved_v<Context, Us, Ts...>))>,
    Context,
    expression<Op, Us...>,
    Ts...>
    : detail::expression_overload<
          argument_indices<expression<Op, Us...>>,
          Context,
          expression<Op, Us...>,
          Ts...>
{};

} // namespace ac
