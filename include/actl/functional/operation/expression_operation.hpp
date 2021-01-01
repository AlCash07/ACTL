// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operation/tuned_operation.hpp>

namespace ac {

template <class... Ts>
struct expression_op : operation<expression_op<Ts...>> {
    std::tuple<Ts...> args;

    template <class... Us>
    explicit constexpr expression_op(Us&&... xs) : args{std::forward<Us>(xs)...} {}

    template <class... Us>
    constexpr auto evaluate(const Us&... xs) const {
        return eval(expand_expression(*this, xs...));
    }

    template <class T, class... Us>
    constexpr void evaluate_to(T& dst, const Us&... xs) const {
        assign(out(dst), expand_expression(*this, xs...));
    }
};

template <class... Ts, enable_int_if<1 < (... + is_operation_v<Ts>)> = 0>
constexpr auto make_expression(Ts&&... xs) {
    return expression_op<value_if_small<Ts>...>{std::forward<Ts>(xs)...};
}

template <class Op, class... Ts>
constexpr decltype(auto) expand_expression(const Op& op, const Ts&... xs) {
    if constexpr (!is_operation_v<Op>)
        return op;
    else
        return op(xs...);
}

template <size_t... Is, class EO, class... Us>
constexpr auto expand_impl(std::index_sequence<Is...>, const EO& eop, const Us&... xs) {
    return make_expression(std::get<0>(eop.args),
                           expand_expression(std::get<Is + 1>(eop.args), xs...)...);
}

template <class... Ts, class... Us>
constexpr auto expand_expression(const expression_op<Ts...>& eop, const Us&... xs) {
    return expand_impl(std::make_index_sequence<sizeof...(Ts) - 1>{}, eop, xs...);
}

template <class EO, class Policy, size_t... Is>
constexpr auto apply_policy_impl(const EO& eop, const Policy policy, std::index_sequence<Is...>) {
    return make_expression(apply_policy_if_can(std::get<Is>(eop.args), policy)...);
}

template <class... Ts, class Policy,
          enable_int_if<(... || can_apply_policy<Ts, Policy>::value)> = 0>
constexpr auto apply_policy(const expression_op<Ts...>& eop, const Policy& policy) {
    return apply_policy_impl(eop, policy, std::make_index_sequence<sizeof...(Ts)>{});
}

}  // namespace ac
