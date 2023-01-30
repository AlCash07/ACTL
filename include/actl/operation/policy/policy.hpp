// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>
#include <utility>

namespace ac {

template <class T>
concept Policy = requires { typename std::remove_reference_t<T>::is_policy; };

template <class Op, class Policy>
inline constexpr bool can_apply_policy_v =
    requires(Op op, Policy policy) { apply_policy(op, policy); };

template <class Op>
constexpr decltype(auto) apply_policy_if_can(Op&& op, Policy auto const&)
{
    return std::forward<Op>(op);
}

template <class Op, Policy P>
    requires can_apply_policy_v<Op, P>
constexpr decltype(auto) apply_policy_if_can(Op&& op, P const& policy)
{
    return apply_policy(std::forward<Op>(op), policy);
}

} // namespace ac
