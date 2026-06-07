// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/overload/resolve_overload.hpp>
#include <actl/operation/policy/policy_stack.hpp>
#include <actl/operation/policy/tuned_operation.hpp>

namespace ac {

template<Operation Op, typename ArgsArray, typename Top, typename Rest>
    requires(!can_apply_any_policy_v<Op, policy_stack<Top, Rest>>)
struct policy_overload<Op, ArgsArray, policy_stack<Top, Rest>>
    : policy_overload<Op, ArgsArray, none> {};

template<Operation Op, typename ArgsArray, typename Top, typename Rest>
struct policy_overload<Op, ArgsArray, policy_stack<Top, Rest>> {
    template<typename Op1>
    static constexpr auto resolve(Op1&& op, policy_stack<Top, Rest> policy) {
        auto&& new_op = apply_policy_if_can(std::forward<Op1>(op), policy.top);
        return overload_resolver<raw_t<decltype(new_op)>, ArgsArray, Rest>::
            resolve(std::forward<decltype(new_op)>(new_op), policy.rest);
    }
};

template<Operation Op, typename NewPolicy, typename ArgsArray, typename Policy>
struct overload_resolver<tuned_operation<Op, NewPolicy>, ArgsArray, Policy> {
    static constexpr decltype(auto) resolve(
        tuned_operation<Op, NewPolicy> const& op, Policy policy
    ) {
        using resolver = overload_resolver<
            raw_t<Op>,
            ArgsArray,
            policy_stack<NewPolicy, Policy>>;
        return resolver::resolve(op.operation, {op.policy, policy});
    }
};

} // namespace ac
