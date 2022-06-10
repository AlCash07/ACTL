// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/expression.hpp>

namespace ac {

template <class Op, class... Ts>
constexpr decltype(auto) pass_arguments(
    Op const& op, [[maybe_unused]] Ts const&... xs)
{
    if constexpr (is_operation_v<Op>)
        if constexpr (is_expression_v<Op>)
            return pass_arguments_impl(argument_indices<Op>{}, op, xs...);
        else
            return op(xs...);
    else
        return op;
}

template <size_t... Is, class OE, class... Ts>
constexpr auto pass_arguments_impl(
    std::index_sequence<Is...>, OE const& oe, Ts const&... xs)
{
    return expression{
        oe.operation(), pass_arguments(std::get<Is + 1>(oe.args), xs...)...};
}

template <class Derived>
struct operation;

template <class Derived>
struct operation_expression<Derived, true> : operation<Derived>
{
    template <class... Ts>
    using result_type = typename expression_result_type<decltype(
        pass_arguments(std::declval<Derived>(), std::declval<Ts>()...))>::type;

    template <class... Ts>
    constexpr auto evaluate(Ts const&... xs) const
    {
        return eval(pass_arguments(this->derived(), xs...));
    }

    template <class T, class... Ts>
    constexpr void evaluate_to(T& dst, Ts const&... xs) const
    {
        assign(out{dst}, pass_arguments(this->derived(), xs...));
    }
};

} // namespace ac
