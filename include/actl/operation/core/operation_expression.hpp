// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/expression.hpp>

namespace ac {

template<typename Op, typename... Ts>
constexpr decltype(auto) pass_arguments(
    Op const& op, [[maybe_unused]] Ts const&... xs
) {
    if constexpr (Operation<Op>)
        if constexpr (is_expression_v<Op>)
            return pass_arguments_impl(argument_indices<Op>{}, op, xs...);
        else
            return op(xs...);
    else
        return op;
}

template<size_t... Is, typename OE, typename... Ts>
constexpr auto pass_arguments_impl(
    std::index_sequence<Is...>, OE const& oe, Ts const&... xs
) {
    return expression{
        oe.operation(), pass_arguments(std::get<Is + 1>(oe.args), xs...)...
    };
}

template<typename Derived>
struct operation;

template<typename Derived>
struct operation_expression<Derived, true> : operation<Derived> {
    template<typename... Ts>
    using result_type = typename expression_result_type<decltype(pass_arguments(
        std::declval<Derived>(), std::declval<Ts>()...
    ))>::type;

    // TODO: deduce exact category here.
    using operation_category = operation_tag;

    template<typename... Ts>
    constexpr auto evaluate(Ts const&... xs) const {
        return eval(pass_arguments(this->derived(), xs...));
    }

    template<typename T, typename... Ts>
    constexpr void evaluate_to(T& target, Ts const&... xs) const {
        assign(out{target}, pass_arguments(this->derived(), xs...));
    }
};

} // namespace ac
