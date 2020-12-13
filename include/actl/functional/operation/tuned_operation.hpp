// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operation/operation_traits.hpp>
#include <tuple>

namespace ac::math {

template <class Op, class Policy>
struct tuned_operation : operation<tuned_operation<Op, Policy>> {
    std::tuple<Op, Policy> t;

    template <class... Ts>
    explicit constexpr tuned_operation(Ts&&... xs) : t{std::forward<Ts>(xs)...} {}
};

template <class T, class = void>
struct is_policy : std::false_type {};

template <class T>
struct is_policy<T, std::void_t<typename T::is_policy>> : std::true_type {};

template <class T>
constexpr bool is_policy_v = is_policy<T>::value;

template <class Op, class Policy, class = void>
struct can_apply_policy : std::false_type {};

template <class Op, class Policy>
struct can_apply_policy<
    Op, Policy, std::void_t<decltype(apply_policy(std::declval<Op>(), std::declval<Policy>()))>>
    : std::true_type {};

template <class Op, class Policy>
constexpr decltype(auto) apply_policy_if_can(Op&& op, const Policy& policy) {
    if constexpr (can_apply_policy<remove_cvref_t<Op>, Policy>::value) {
        return apply_policy(std::forward<Op>(op), policy);
    } else {
        return std::forward<Op>(op);
    }
}

}  // namespace ac::math
