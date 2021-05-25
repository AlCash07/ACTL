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
    const Policy& policy;
};

template <
    class... Ts,
    class Base,
    class Policy,
    class Op,
    enable_int_if<
        is_primary_overload_resolved_v<Op, Ts...> &&
        can_apply_policy_v<Op, Policy>> = 0>
constexpr auto resolve_overload(policy_context<Base, Policy> context, Op&& op)
{
    auto result = apply_policy(std::forward<Op>(op), context.policy);
    if constexpr (is_overload_resolved_v<Base, decltype(result), Ts...>)
        return std::move(result);
    else
        return resolve_overload<Ts...>(Base{context}, std::move(result));
}

template <class... Ts, class Context, class Op, class Policy>
constexpr decltype(auto) resolve_overload(
    Context context, const tuned_operation<Op, Policy>& op)
{
    using PolicyContext = policy_context<Context, Policy>;
    if constexpr (is_overload_resolved_v<PolicyContext, Op, Ts...>)
        return op.operation;
    else
        return resolve_overload<Ts...>(
            PolicyContext{context, op.policy}, op.operation);
}

} // namespace ac
