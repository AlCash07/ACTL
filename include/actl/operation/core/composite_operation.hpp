// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/operation.hpp>

namespace ac {

template <class OuterOp, class... InnerOps>
struct composite_operation
    : operation<composite_operation<OuterOp, InnerOps...>>
    , private std::tuple<InnerOps...>
{
    template <class... Ts>
    explicit constexpr composite_operation(Ts&&... xs)
        : std::tuple<InnerOps...>{std::forward<Ts>(xs)...}
    {}

    constexpr std::tuple<InnerOps...> const& inner() const
    {
        return *this;
    }

    template <class... Ts>
    constexpr auto evaluate(Ts const&... xs) const
    {
        if constexpr (sizeof...(InnerOps) == 1)
            return OuterOp::evaluate(std::get<0>(inner()), xs...);
        else
            return OuterOp::evaluate(inner(), xs...);
    }
};

template <class OuterOp>
struct operation_composer
{
    template <class... InnerOps>
    constexpr auto operator()(InnerOps&&... ops) const
    {
        constexpr index N = OuterOp::inner_count;
        static_assert(N == -1 || N == index{sizeof...(InnerOps)});
        return composite_operation<OuterOp, value_if_small<InnerOps>...>{
            std::forward<InnerOps>(ops)...};
    }
};

template <class Outer, class... Inner, class Policy, size_t... Is>
constexpr auto apply_policy_to_composite(
    composite_operation<Outer, Inner...> const& op,
    Policy const& policy,
    std::index_sequence<Is...>)
{
    return operation_composer<Outer>{}(
        apply_policy_if_can(std::get<Is>(op.inner()), policy)...);
}

template <
    class Outer,
    class... Inner,
    class Policy,
    enable_int_if<(... || can_apply_policy<Inner, Policy>::value)> = 0>
constexpr auto apply_policy(
    composite_operation<Outer, Inner...> const& op, Policy const& policy)
{
    return apply_policy_to_composite(
        op, policy, std::make_index_sequence<sizeof...(Inner)>{});
}

} // namespace ac
