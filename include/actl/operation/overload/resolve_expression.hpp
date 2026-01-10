// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/expression/expression.hpp>

namespace ac {

namespace detail {

template<typename OE, typename ArgsArray, typename Context>
struct expression_overload;

template<
    Operation Op,
    size_t... Is,
    typename... Ts,
    typename ArgsArray,
    typename Context>
struct expression_overload<
    expression_data<Op, std::index_sequence<Is...>, Ts...>,
    ArgsArray,
    Context> {
    template<typename OE, typename... Us>
    static constexpr auto make_expression(
        Context context, OE&& oe, Us&&... xs
    ) {
        return expression{
            resolve_overload<Us...>(context, std::forward<OE>(oe)),
            std::forward<Us>(xs)...
        };
    }

    template<typename OE>
    static constexpr auto resolve(Context context, OE&& oe) {
        return make_expression(
            context,
            static_cast<OE&&>(oe).operation,
            overload_resolver<raw_t<Ts>, ArgsArray, Context>::resolve(
                context, std::get<Is>(static_cast<OE&&>(oe).arguments)
            )...
        );
    }
};

} // namespace detail

template<Operation Op, typename... Ts, typename ArgsArray, typename Context>
    requires(!(
        is_overload_resolved_v<raw_t<Op>, type_array<raw_t<Ts>...>, Context> &&
        (... && is_overload_resolved_v<raw_t<Ts>, ArgsArray, Context>)
    ))
struct overload_resolver<expression<Op, Ts...>, ArgsArray, Context>
    : detail::expression_overload<
          expression_data_t<Op, Ts...>,
          ArgsArray,
          Context> {};

} // namespace ac
