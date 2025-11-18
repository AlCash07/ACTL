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
        oe.operation, pass_arguments(std::get<Is>(oe.arguments), args...)...
    };
}

template<typename Derived>
struct operation_base;

template<typename Operation, typename... Args>
    requires(... || ac::Operation<std::remove_cvref_t<Args>>)
struct expression<Operation, Args...>
    : expression_storage<Operation, Args...>
    , operation_base<expression<Operation, Args...>> {
    template<typename... Ts>
    using result_type = typename expression_result_type<decltype(pass_arguments(
        std::declval<expression>(), std::declval<Ts>()...
    ))>::type;

    using expression_storage<Operation, Args...>::expression_storage;

    template<typename... Ts>
    constexpr auto evaluate(Ts const&... args) const {
        return eval(pass_arguments(*this, args...));
    }

    template<typename T, typename... Ts>
    constexpr void evaluate_to(T& target, Ts const&... args) const {
        assign(out{target}, pass_arguments(*this, args...));
    }
};

} // namespace ac
