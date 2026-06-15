// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/operation/Operation.hpp>
#include <type_traits>

namespace ac {

template<typename T>
concept Policy = requires { typename std::remove_reference_t<T>::is_policy; };

template<typename ArgsArray, Operation Op, typename Policy>
inline constexpr bool can_apply_policy_v =
    requires(Op op, Policy const& policy) {
        apply_policy(op, policy, ArgsArray{});
    };

template<typename ArgsArray, Operation Op, typename... Policies>
inline constexpr bool can_apply_any_policy_v =
    (... || can_apply_policy_v<ArgsArray, Op, Policies>);

} // namespace ac
