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

template<Operation Op, typename ArgsArray, typename Head, typename Tail>
    requires(!can_apply_any_policy_v<Op, policy_stack<Head, Tail>>)
struct policy_overload<Op, ArgsArray, policy_stack<Head, Tail>>
    : policy_overload<Op, ArgsArray, none> {};

template<Operation Op, typename ArgsArray, typename Head, typename Tail>
struct policy_overload<Op, ArgsArray, policy_stack<Head, Tail>> {
    template<typename Op1>
    static constexpr auto resolve(Op1&& op, policy_stack<Head, Tail> policy) {
        auto&& new_op = apply_policy_if_can(std::forward<Op1>(op), policy.head);
        return overload_resolver<raw_t<decltype(new_op)>, ArgsArray, Tail>::
            resolve(std::forward<decltype(new_op)>(new_op), policy.tail);
    }
};

template<Operation Op, typename OpPolicy, typename ArgsArray, typename Policy>
struct overload_resolver<tuned_operation<Op, OpPolicy>, ArgsArray, Policy> {
    static constexpr decltype(auto) resolve(
        tuned_operation<Op, OpPolicy> const& op, Policy policy
    ) {
        using resolver = overload_resolver<
            raw_t<Op>,
            ArgsArray,
            policy_stack<OpPolicy, Policy>>;
        return resolver::resolve(op.operation, {op.policy, policy});
    }
};

} // namespace ac
