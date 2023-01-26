// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/policy/policy.hpp>

namespace ac {

template <class Op, class Policy>
struct tuned_operation : operation<tuned_operation<Op, Policy>>
{
    // [[no_unique_address]]
    Op operation;
    Policy policy;
};

template <
    class Op,
    class Policy,
    enable_int_if<
        Operation<std::remove_cvref_t<Op>> &&
        is_policy_v<std::remove_cvref_t<Policy>>> = 0>
constexpr auto operator|(Op&& op, Policy&& policy)
{
    return tuned_operation<value_if_small<Op>, value_if_small<Policy>>{
        {}, std::forward<Op>(op), std::forward<Policy>(policy)};
}

} // namespace ac
