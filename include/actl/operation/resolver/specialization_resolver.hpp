// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/policy/policy.hpp>
#include <actl/operation/resolver/constant_resolver.hpp>

namespace ac {

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

template<Operation Op, typename ArgsArray>
struct specialization_error {
    // Error here means that the operation cannot be evaluated for the given
    // arguments, and there are no specializations found to mitigate that.
    // Dummy type is defined to produce a helpful compilation error that
    // explains why all the found specializations didn't match.
    using _ = decltype(specialization(std::declval<Op>(), ArgsArray{}));
};

template<Operation Op, typename... Args, typename... Policies>
    requires(
        !requires(Op op, Args&&... args) { op.evaluate(args...); } &&
        !requires(Op op) { specialization(op, type_array<Args...>{}); }
    )
struct specialization_resolver<Op, type_array<Args...>, Policies...>
    : std::conditional_t<
          (... || is_constant_v<Args>),
          constant_resolver<
              Op,
              type_array<Args...>,
              std::index_sequence_for<Args...>,
              Policies...>,
          specialization_error<Op, type_array<Args...>>> {};

} // namespace ac
