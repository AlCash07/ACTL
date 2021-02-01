// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/expression.hpp>
#include <actl/operation/core/policy.hpp>

namespace ac {

template <class Derived>
struct expression_base<Derived, operation_tag> {
    struct type : operation<Derived> {
        template <class... Ts>
        constexpr auto evaluate(const Ts&... xs) const {
            return eval(expand_expression(this->derived(), xs...));
        }

        template <class T, class... Ts>
        constexpr void evaluate_to(T& dst, const Ts&... xs) const {
            assign(out(dst), expand_expression(this->derived(), xs...));
        }
    };
};

template <class Op, class... Ts>
constexpr decltype(auto) expand_expression(
    const Op& op, [[maybe_unused]] const Ts&... xs) //
{
    if constexpr (is_operation_v<Op>)
        if constexpr (is_expression<Op>::value)
            return expand_impl(
                std::make_index_sequence<Op::argument_count>{}, op, xs...);
        else
            return op(xs...);
    else
        return op;
}

template <size_t... Is, class EOp, class... Ts>
constexpr auto expand_impl(
    std::index_sequence<Is...>, const EOp& eop, const Ts&... xs) //
{
    return make_expression(
        eop.operation(),
        expand_expression(std::get<Is + 1>(eop.args), xs...)...);
}

template <class EOp, class Policy, size_t... Is>
constexpr auto apply_policy_impl(
    const EOp& eop, const Policy policy, std::index_sequence<Is...>) //
{
    return make_expression(
        apply_policy_if_can(std::get<Is>(eop.args), policy)...);
}

template <
    class... Ts,
    class Policy,
    enable_int_if<
        is_operation_v<expression<Ts...>> &&
        (... || can_apply_policy<Ts, Policy>::value)> = 0>
constexpr auto apply_policy(
    const expression<Ts...>& eop, const Policy& policy) //
{
    return apply_policy_impl(
        eop, policy, std::make_index_sequence<sizeof...(Ts)>{});
}

} // namespace ac
