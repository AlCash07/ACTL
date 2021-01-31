// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/operation_traits.hpp>
#include <actl/operation/core/policy.hpp>

namespace ac {

template <class Op, class Policy>
struct tuned_operation : operation<tuned_operation<Op, Policy>> {
    // [[no_unique_address]]
    Op operation;
    Policy policy;
};

template <
    class Op,
    class Policy,
    enable_int_if<
        is_operation_v<remove_cvref_t<Op>> &&
        is_policy_v<remove_cvref_t<Policy>>> = 0>
constexpr auto operator|(Op&& op, Policy&& policy) {
    return tuned_operation<Op, Policy>{
        {}, std::forward<Op>(op), std::forward<Policy>(policy)};
}

template <class Op, class Policy, class Category, class... Ts>
struct overload<tuned_operation<Op, Policy>, Category, Ts...> {
    static constexpr decltype(auto) resolve(
        const tuned_operation<Op, Policy>& op) {
        return apply_policy_if_can(ac::resolve<Ts...>(op.operation), op.policy);
    }
};

} // namespace ac
