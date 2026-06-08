// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/resolver/operation_resolver.hpp>
#include <actl/operation/policy/tuned_operation.hpp>

namespace ac {

template<
    Operation Op,
    typename ArgsArray,
    typename TopPolicy,
    typename... Policies>
    requires can_apply_any_policy_v<Op, TopPolicy, Policies...>
struct policy_overload<Op, ArgsArray, TopPolicy, Policies...> {
    template<typename Op1>
    static constexpr auto resolve(
        Op1&& op, TopPolicy const& top_policy, Policies const&... policies
    ) {
        auto&& new_op = apply_policy_if_can(std::forward<Op1>(op), top_policy);
        using resolver =
            operation_resolver<raw_t<decltype(new_op)>, ArgsArray, Policies...>;
        return resolver::resolve(
            std::forward<decltype(new_op)>(new_op), policies...
        );
    }
};

template<
    Operation Op,
    typename NewPolicy,
    typename ArgsArray,
    typename... Policies>
struct operation_resolver<
    tuned_operation<Op, NewPolicy>,
    ArgsArray,
    Policies...> {
    static constexpr decltype(auto) resolve(
        tuned_operation<Op, NewPolicy> const& op, Policies const&... policies
    ) {
        using resolver =
            operation_resolver<raw_t<Op>, ArgsArray, NewPolicy, Policies...>;
        return resolver::resolve(op.operation, op.policy, policies...);
    }
};

} // namespace ac
