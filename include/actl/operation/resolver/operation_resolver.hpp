// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/policy/policy.hpp>
#include <actl/operation/resolver/specialization.hpp>

namespace ac {

template<typename Op, typename ArgsArray, typename... Policies>
struct policy_overload {
    struct is_resolved;

    constexpr Op operator()(Op&& op, Policies const&...) const {
        return std::move(op);
    }

    constexpr Op const& operator()(Op const& op, Policies const&...) const {
        return op;
    }
};

template<typename Op, typename ArgsArray, typename... Policies>
struct operation_resolver : policy_overload<Op, ArgsArray, Policies...> {};

template<
    Operation Op,
    typename ArgsArray,
    typename TopPolicy,
    typename... Policies>
    requires can_apply_any_policy_v<Op, TopPolicy, Policies...>
struct policy_overload<Op, ArgsArray, TopPolicy, Policies...> {
    template<typename Op1>
    constexpr auto operator()(
        Op1&& op, TopPolicy const& top_policy, Policies const&... policies
    ) const {
        auto&& new_op = apply_policy_if_can(std::forward<Op1>(op), top_policy);
        using resolver =
            operation_resolver<raw_t<decltype(new_op)>, ArgsArray, Policies...>;
        return resolver{}(std::forward<decltype(new_op)>(new_op), policies...);
    }
};

template<typename Op, typename... Args, typename... Policies>
    requires requires { specialization<Op, Args...>::formula; }
struct operation_resolver<Op, type_array<Args...>, Policies...> {
    template<typename Op1>
    constexpr auto operator()(Op1&& op, Policies const&... policies) const {
        using Formula =
            std::remove_const_t<decltype(specialization<Op, Args...>::formula)>;
        return operation_resolver<Formula, type_array<Args...>, Policies...>{}(
            Formula{}, policies...
        );
    }
};

template<Operation Op, typename... Args>
inline constexpr auto resolve_operation =
    operation_resolver<raw_t<Op>, type_array<raw_t<Args>...>>{};

template<Operation Op, typename ArgsArray, typename... Policies>
inline constexpr bool is_operation_resolved_v = requires {
    typename operation_resolver<raw_t<Op>, ArgsArray, Policies...>::is_resolved;
};

} // namespace ac
