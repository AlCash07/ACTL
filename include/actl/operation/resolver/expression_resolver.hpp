// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/expression/expression.hpp>

namespace ac {

namespace detail {

template<typename OE, typename ArgsArray, typename... Policies>
struct expression_resolver;

template<
    Operation Op,
    size_t... Is,
    typename... Ts,
    typename ArgsArray,
    typename... Policies>
struct expression_resolver<
    expression_data<Op, std::index_sequence<Is...>, Ts...>,
    ArgsArray,
    Policies...> {
    template<typename OE, typename... Us>
    static constexpr auto make_expression(
        OE&& oe, Policies const&... policies, Us&&... xs
    ) {
        return expression{
            operation_resolver<
                raw_t<OE>,
                type_array<raw_t<Us>...>,
                Policies...>{}(std::forward<OE>(oe), policies...),
            std::forward<Us>(xs)...
        };
    }

    template<typename OE>
    constexpr auto operator()(OE&& oe, Policies const&... policies) const {
        return make_expression(
            static_cast<OE&&>(oe).operation,
            policies...,
            operation_resolver<raw_t<Ts>, ArgsArray, Policies...>{}(
                std::get<Is>(static_cast<OE&&>(oe).arguments), policies...
            )...
        );
    }
};

} // namespace detail

template<Operation Op, typename... Ts, typename ArgsArray, typename... Policies>
    requires(
        !(is_operation_resolved_v<
              raw_t<Op>,
              type_array<raw_t<Ts>...>,
              Policies...> &&
          (... && is_operation_resolved_v<raw_t<Ts>, ArgsArray, Policies...>))
    )
struct operation_resolver<expression<Op, Ts...>, ArgsArray, Policies...>
    : detail::expression_resolver<
          expression_data_t<Op, Ts...>,
          ArgsArray,
          Policies...> {};

} // namespace ac
