// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/policy/expression_operation.hpp>
#include <actl/operation/policy/tuned_operation.hpp>

namespace ac {

template <class... Ts>
struct resolve_under_policy {
    template <
        class Op,
        class Policy,
        class... Policies,
        enable_int_if<!is_expression_v<Op>> = 0>
    static constexpr decltype(auto) resolve(
        Op&& op, const Policy& policy, const Policies&... policies) //
    {
        if constexpr (is_overload_resolved_v<Op, Ts...>)
            if constexpr (sizeof...(Policies) == 0)
                return ac::resolve<Ts...>(
                    apply_policy_if_can(std::forward<Op>(op), policy));
            else
                return resolve(
                    apply_policy_if_can(std::forward<Op>(op), policy),
                    policies...);
        else
            return resolve(
                ac::resolve<Ts...>(std::forward<Op>(op)), policy, policies...);
    }

    template <class Op, class Policy, class... Policies>
    static constexpr decltype(auto) resolve(
        const tuned_operation<Op, Policy>& op, const Policies&... policies) //
    {
        return resolve(op.operation, op.policy, policies...);
    }

    template <class EOp, class... Policies, size_t... Is>
    static constexpr decltype(auto) resolve_expression(
        std::index_sequence<Is...>,
        EOp&& eop,
        const Policies&... policies) //
    {
        using resolver = resolve_under_policy<decltype(resolve(
            std::get<Is + 1>(static_cast<EOp&&>(eop).args), policies...))...>;
        return resolver::resolve(
            static_cast<EOp&&>(eop).operation(), policies...)(resolve(
            std::get<Is + 1>(static_cast<EOp&&>(eop).args), policies...)...);
    }

    template <
        class EOp,
        class... Policies,
        enable_int_if<is_expression_v<EOp>> = 0>
    static constexpr decltype(auto) resolve(
        EOp&& eop, const Policies&... policies) //
    {
        return resolve_expression(
            argument_indices<EOp>{}, std::forward<EOp>(eop), policies...);
    }
};

template <class Op, class Policy, class Category, class... Ts>
struct overload<tuned_operation<Op, Policy>, Category, Ts...> {
    static constexpr decltype(auto) resolve(
        const tuned_operation<Op, Policy>& op) //
    {
        return resolve_under_policy<Ts...>::resolve(op);
    }
};

} // namespace ac
