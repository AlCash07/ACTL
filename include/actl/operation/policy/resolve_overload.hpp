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

template<typename Base, typename Policy, Operation Op, typename... Ts>
    requires can_apply_policy_v<Op, Policy>
struct context_overload<policy_context<Base, Policy>, Op, Ts...> {
    template<typename Op1>
    static constexpr auto resolve(
        policy_context<Base, Policy> context, Op1&& op
    ) {
        return resolve_overload<Ts...>(
            Base{context}, apply_policy(std::forward<Op1>(op), context.policy)
        );
    }
};

template<typename Context, Operation Op, typename Policy, typename... Ts>
struct overload_resolver<Context, tuned_operation<Op, Policy>, Ts...> {
    static constexpr decltype(auto) resolve(
        Context context, tuned_operation<Op, Policy> const& op
    ) {
        return resolve_overload<Ts...>(
            policy_context<Context, Policy>{context, op.policy}, op.operation
        );
    }
};

} // namespace ac
