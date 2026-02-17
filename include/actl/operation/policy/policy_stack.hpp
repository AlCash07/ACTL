// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/policy/policy.hpp>

namespace ac {

template<typename Head, typename Tail>
struct policy_stack {
    Head const& head;
    Tail const& tail;
};

template<Operation Op, typename Policy>
inline constexpr bool can_apply_any_policy_v = false;

template<Operation Op, typename Head, typename Tail>
inline constexpr bool can_apply_any_policy_v<Op, policy_stack<Head, Tail>> =
    can_apply_policy_v<Op, Head> || can_apply_any_policy_v<Op, Tail>;

} // namespace ac
