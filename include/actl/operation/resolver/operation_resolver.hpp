// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/policy/policy.hpp>
#include <actl/operation/resolver/specialization.hpp>

namespace ac {

struct identity_resolver {
    struct is_resolved;

    template<typename T, typename... Policies>
    constexpr T operator()(T&& t, Policies const&...) const {
        return std::forward<T>(t);
    }
};

template<Operation Op, typename ArgsArray, typename... Policies>
struct specialization_resolver : identity_resolver {};

template<Operation Op, typename ArgsArray, typename... Policies>
struct policy_resolver : specialization_resolver<Op, ArgsArray, Policies...> {};

template<typename Op, typename ArgsArray, typename... Policies>
struct operation_resolver : policy_resolver<Op, ArgsArray, Policies...> {};

template<typename Op, typename... Args>
inline constexpr auto resolve_operation =
    operation_resolver<raw_t<Op>, type_array<raw_t<Args>...>>{};

template<typename Op, typename ArgsArray, typename... Policies>
inline constexpr bool is_operation_resolved_v = requires {
    typename operation_resolver<raw_t<Op>, ArgsArray, Policies...>::is_resolved;
};

/* implementation */

// Operation expressions can contain leaf nodes with values
// instead of operations. Their resolution is handled here.
template<typename T, typename ArgsArray, typename... Policies>
    requires(!Operation<T>)
struct operation_resolver<T, ArgsArray, Policies...> : identity_resolver {};

template<
    Operation Op,
    typename ArgsArray,
    typename TopPolicy,
    typename... Policies>
    requires can_apply_any_policy_v<ArgsArray, Op, TopPolicy, Policies...>
struct policy_resolver<Op, ArgsArray, TopPolicy, Policies...> {
    template<typename Op1>
    constexpr auto operator()(
        Op1&& op, TopPolicy const& top_policy, Policies const&... policies
    ) const {
        auto&& new_op = [&] {
            if constexpr (can_apply_policy_v<ArgsArray, Op, TopPolicy>)
                return apply_policy(
                    std::forward<Op1>(op), top_policy, ArgsArray{}
                );
            else
                return std::forward<Op1>(op);
        }();
        using resolver =
            operation_resolver<raw_t<decltype(new_op)>, ArgsArray, Policies...>;
        return resolver{}(std::forward<decltype(new_op)>(new_op), policies...);
    }
};

template<Operation Op, typename ArgsArray, typename... Policies>
    requires requires { specialization(std::declval<Op>(), ArgsArray{}); }
struct specialization_resolver<Op, ArgsArray, Policies...> {
    template<typename Op1>
    constexpr auto operator()(
        Op1&& op, Policies const&... policies
    ) const noexcept {
        auto&& new_op = specialization(std::forward<Op1>(op), ArgsArray{});
        return operation_resolver<
            raw_t<decltype(new_op)>,
            ArgsArray,
            Policies...>{}(std::forward<decltype(new_op)>(new_op), policies...);
    }
};

} // namespace ac
