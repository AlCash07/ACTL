// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/overload/resolve_overload.hpp>
#include <actl/operation/policy/tuned_operation.hpp>

namespace ac {

template <class Base, class Policy>
struct policy_context : Base
{
    Policy const& policy;
};

template <class Base, class Policy, class Op, class... Ts>
struct context_overload<
    std::enable_if_t<can_apply_policy_v<Op, Policy>>,
    policy_context<Base, Policy>,
    Op,
    Ts...>
{
    template <class Op1>
    static constexpr auto resolve(
        policy_context<Base, Policy> context, Op1&& op)
    {
        return resolve_overload<Ts...>(
            Base{context}, apply_policy(std::forward<Op1>(op), context.policy));
    }
};

template <class Context, class Op, class Policy, class... Ts>
struct overload_resolver<void, Context, tuned_operation<Op, Policy>, Ts...>
{
    static constexpr decltype(auto) resolve(
        Context context, tuned_operation<Op, Policy> const& op)
    {
        return resolve_overload<Ts...>(
            policy_context<Context, Policy>{context, op.policy}, op.operation);
    }
};

} // namespace ac
