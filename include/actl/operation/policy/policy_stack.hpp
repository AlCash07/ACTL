// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/policy/policy.hpp>

namespace ac {

template<typename Top, typename Rest>
struct policy_stack {
    Top const& top;
    Rest const& rest;
};

template<Operation Op, typename Policy>
inline constexpr bool can_apply_any_policy_v = false;

template<Operation Op, typename Top, typename Rest>
inline constexpr bool can_apply_any_policy_v<Op, policy_stack<Top, Rest>> =
    can_apply_policy_v<Op, Top> || can_apply_any_policy_v<Op, Rest>;

} // namespace ac
