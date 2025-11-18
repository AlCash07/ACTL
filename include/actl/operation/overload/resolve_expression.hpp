// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/expression/operation_expression.hpp>

namespace ac {

namespace detail {

template<typename S, typename Context, Operation Op, typename... Ts>
struct expression_overload;

template<size_t... Is, typename Context, Operation Op, typename... Ts>
struct expression_overload<std::index_sequence<Is...>, Context, Op, Ts...> {
    template<typename Op1, typename... Us>
    static constexpr auto make_expression(
        Context context, Op1&& op, Us&&... xs
    ) {
        return expression{
            resolve_overload<Us...>(context, std::forward<Op1>(op)),
            std::forward<Us>(xs)...
        };
    }

    template<typename OE>
    static constexpr auto resolve(Context context, OE&& oe) {
        return make_expression(
            context,
            static_cast<OE&&>(oe).operation,
            resolve_overload<Ts...>(
                context, std::get<Is>(static_cast<OE&&>(oe).arguments)
            )...
        );
    }
};

} // namespace detail

template<typename Context, Operation Op, typename... Us, typename... Ts>
    requires(
        !(is_overload_resolved_v<Context, Op, Us...> &&
          (... && is_overload_resolved_v<Context, Us, Ts...>))
    )
struct overload_resolver<Context, expression<Op, Us...>, Ts...>
    : detail::expression_overload<
          std::index_sequence_for<Us...>,
          Context,
          expression<Op, Us...>,
          Ts...> {};

} // namespace ac
