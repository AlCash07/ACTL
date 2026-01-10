// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/overload/resolve_overload.hpp>
#include <actl/operation/policy/tuned_operation.hpp>

namespace ac {

template<typename Base, typename P>
struct policy_context : Base {
    P const& policy;
};

template<Operation Op, typename ArgsArray, typename Base, typename Policy>
    requires can_apply_policy_v<Op, Policy>
struct context_overload<Op, ArgsArray, policy_context<Base, Policy>> {
    template<typename Op1>
    static constexpr auto resolve(
        policy_context<Base, Policy> context, Op1&& op
    ) {
        auto&& new_op = apply_policy(std::forward<Op1>(op), context.policy);
        return overload_resolver<raw_t<decltype(new_op)>, ArgsArray, Base>::
            resolve(Base{context}, std::forward<decltype(new_op)>(new_op));
    }
};

template<Operation Op, typename Policy, typename ArgsArray, typename Context>
struct overload_resolver<tuned_operation<Op, Policy>, ArgsArray, Context> {
    static constexpr decltype(auto) resolve(
        Context context, tuned_operation<Op, Policy> const& op
    ) {
        using resolver = overload_resolver<
            raw_t<Op>,
            ArgsArray,
            policy_context<Context, Policy>>;
        return resolver::resolve({context, op.policy}, op.operation);
    }
};

} // namespace ac
