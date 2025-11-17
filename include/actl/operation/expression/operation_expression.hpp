// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/expression/expression.hpp>

namespace ac {

template<typename Op, typename... Args>
constexpr decltype(auto) pass_arguments(
    Op const& op, [[maybe_unused]] Args const&... args
) {
    if constexpr (Operation<Op>)
        if constexpr (is_expression_v<Op>)
            return pass_arguments_impl(argument_indices<Op>{}, op, args...);
        else
            return op(args...);
    else
        return op;
}

template<size_t... Is, typename OE, typename... Args>
constexpr auto pass_arguments_impl(
    std::index_sequence<Is...>, OE const& oe, Args const&... args
) {
    return expression{
        oe.operation(), pass_arguments(std::get<Is + 1>(oe.args), args...)...
    };
}

template<typename Derived>
struct operation;

template<typename Derived>
struct operation_expression<Derived, true> : operation<Derived> {
    template<typename... Args>
    using result_type = typename expression_result_type<decltype(pass_arguments(
        std::declval<Derived>(), std::declval<Args>()...
    ))>::type;

    // TODO: deduce exact category here.
    using operation_category = operation_tag;

    template<typename... Args>
    constexpr auto evaluate(Args const&... args) const {
        return eval(pass_arguments(this->derived(), args...));
    }

    template<typename T, typename... Args>
    constexpr void evaluate_to(T& target, Args const&... args) const {
        assign(out{target}, pass_arguments(this->derived(), args...));
    }
};

} // namespace ac
