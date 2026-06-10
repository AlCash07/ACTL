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
    typename... StoredArgs,
    typename ArgsArray,
    typename... Policies>
struct expression_resolver<
    expression_data<Op, std::index_sequence<Is...>, StoredArgs...>,
    ArgsArray,
    Policies...> {
    template<typename Op1, typename... ResolvedArgs>
    static constexpr auto make_expression(
        Op1&& op, Policies const&... policies, ResolvedArgs&&... args
    ) {
        return expression{
            operation_resolver<
                Op,
                type_array<raw_t<ResolvedArgs>...>,
                Policies...>{}(std::forward<Op1>(op), policies...),
            std::forward<ResolvedArgs>(args)...
        };
    }

    template<typename OE>
    constexpr auto operator()(OE&& oe, Policies const&... policies) const {
        return make_expression(
            static_cast<OE&&>(oe).operation,
            policies...,
            operation_resolver<raw_t<StoredArgs>, ArgsArray, Policies...>{}(
                std::get<Is>(static_cast<OE&&>(oe).arguments), policies...
            )...
        );
    }
};

} // namespace detail

template<
    Operation Op,
    typename... StoredArgs,
    typename ArgsArray,
    typename... Policies>
    requires(
        !(is_operation_resolved_v<
              raw_t<Op>,
              type_array<raw_t<StoredArgs>...>,
              Policies...> &&
          (... &&
           is_operation_resolved_v<raw_t<StoredArgs>, ArgsArray, Policies...>))
    )
struct operation_resolver<expression<Op, StoredArgs...>, ArgsArray, Policies...>
    : detail::expression_resolver<
          expression_data_t<Op, StoredArgs...>,
          ArgsArray,
          Policies...> {};

} // namespace ac
