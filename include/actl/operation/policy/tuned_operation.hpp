// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/expression/value_if_cheap.hpp>
#include <actl/operation/policy/policy.hpp>
#include <actl/operation/resolver/operation_resolver.hpp>

namespace ac {

template<Operation Op, typename Policy>
struct tuned_operation : operation_base<tuned_operation<Op, Policy>> {
    // [[no_unique_address]]
    Op operation;
    Policy policy;
};

template<Operation Op, Policy P>
constexpr auto operator|(Op&& op, P&& policy) {
    return tuned_operation<value_if_cheap_t<Op>, value_if_cheap_t<P>>{
        {}, std::forward<Op>(op), std::forward<P>(policy)
    };
}

template<
    Operation Op,
    typename NewPolicy,
    typename ArgsArray,
    typename... Policies>
struct operation_resolver<
    tuned_operation<Op, NewPolicy>,
    ArgsArray,
    Policies...> {
    constexpr decltype(auto) operator()(
        tuned_operation<Op, NewPolicy> const& op, Policies const&... policies
    ) const {
        using resolver =
            operation_resolver<raw_t<Op>, ArgsArray, NewPolicy, Policies...>;
        return resolver{}(op.operation, op.policy, policies...);
    }
};

} // namespace ac
