// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/policy/policy.hpp>

namespace ac {

template <class EOp, class Policy, size_t... Is>
constexpr auto apply_policy_impl(
    const EOp& eop, const Policy policy, std::index_sequence<Is...>) //
{
    return expression{apply_policy_if_can(std::get<Is>(eop.args), policy)...};
}

template <
    class... Ts,
    class Policy,
    enable_int_if<
        is_operation_v<expression<Ts...>> &&
        (... || can_apply_policy<Ts, Policy>::value)> = 0>
constexpr auto apply_policy(
    const expression<Ts...>& eop, const Policy& policy) //
{
    return apply_policy_impl(
        eop, policy, std::make_index_sequence<sizeof...(Ts)>{});
}

} // namespace ac
