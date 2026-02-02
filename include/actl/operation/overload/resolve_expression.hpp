// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/expression/expression.hpp>

namespace ac {

namespace detail {

template<typename OE, typename ArgsArray, typename Policy>
struct expression_overload;

template<
    Operation Op,
    size_t... Is,
    typename... Ts,
    typename ArgsArray,
    typename Policy>
struct expression_overload<
    expression_data<Op, std::index_sequence<Is...>, Ts...>,
    ArgsArray,
    Policy> {
    template<typename OE, typename... Us>
    static constexpr auto make_expression(OE&& oe, Policy policy, Us&&... xs) {
        return expression{
            overload_resolver<raw_t<OE>, type_array<raw_t<Us>...>, Policy>::
                resolve(std::forward<OE>(oe), policy),
            std::forward<Us>(xs)...
        };
    }

    template<typename OE>
    static constexpr auto resolve(OE&& oe, Policy policy) {
        return make_expression(
            static_cast<OE&&>(oe).operation,
            policy,
            overload_resolver<raw_t<Ts>, ArgsArray, Policy>::resolve(
                std::get<Is>(static_cast<OE&&>(oe).arguments), policy
            )...
        );
    }
};

} // namespace detail

template<Operation Op, typename... Ts, typename ArgsArray, typename Policy>
    requires(
        !(is_overload_resolved_v<raw_t<Op>, type_array<raw_t<Ts>...>, Policy> &&
          (... && is_overload_resolved_v<raw_t<Ts>, ArgsArray, Policy>))
    )
struct overload_resolver<expression<Op, Ts...>, ArgsArray, Policy>
    : detail::
          expression_overload<expression_data_t<Op, Ts...>, ArgsArray, Policy> {
};

} // namespace ac
