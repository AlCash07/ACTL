// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/operation_traits.hpp>
#include <actl/operation/core/policy.hpp>
#include <tuple>

namespace ac {

template <class Op, class Policy>
class tuned_operation : public operation<tuned_operation<Op, Policy>> {
    std::tuple<Op, Policy> t;

public:
    template <class... Ts>
    explicit constexpr tuned_operation(Ts&&... xs) : t{std::forward<Ts>(xs)...} {}

    template <class... Ts>
    constexpr decltype(auto) resolve() const {
        return apply_policy_if_can(std::get<0>(t).template resolve<Ts...>(), std::get<1>(t));
    }
};

template <
    class Op,
    class Policy,
    enable_int_if<is_operation_v<remove_cvref_t<Op>> && is_policy_v<remove_cvref_t<Policy>>> = 0>
constexpr auto operator|(Op&& op, Policy&& policy) {
    return tuned_operation<Op, Policy>{std::forward<Op>(op), std::forward<Policy>(policy)};
}

} // namespace ac
