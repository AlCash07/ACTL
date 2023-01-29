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

template <class Op, class Policy, class = void>
struct can_apply_policy : std::false_type
{};

template <class Op, class Policy>
struct can_apply_policy<
    Op,
    Policy,
    std::void_t<decltype(apply_policy(
        std::declval<Op>(), std::declval<Policy>()))>> : std::true_type
{};

template <class Op, class Policy>
inline constexpr bool can_apply_policy_v = can_apply_policy<Op, Policy>::value;

template <class Op, Policy P>
constexpr decltype(auto) apply_policy_if_can(Op&& op, P const& policy)
{
    if constexpr (can_apply_policy<Op, P>::value)
        return apply_policy(std::forward<Op>(op), policy);
    else
        return std::forward<Op>(op);
}

} // namespace ac
