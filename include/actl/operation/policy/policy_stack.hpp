// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/core/none.hpp>
#include <actl/operation/policy/policy.hpp>

namespace ac {

template<typename Head, typename Tail>
struct policy_stack {
    Head const& head;
    Tail const& tail;
};

template<typename Op>
constexpr none skip_irrelevant(none) noexcept {
    return {};
}

template<typename Op, typename Head, typename Tail>
constexpr auto skip_irrelevant(policy_stack<Head, Tail> policy_stack) noexcept {
    if constexpr (can_apply_policy_v<Op, Head>)
        return policy_stack;
    else
        return skip_irrelevant<Op>(policy_stack.tail);
}

} // namespace ac
