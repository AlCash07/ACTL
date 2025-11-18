// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/expression/expression.hpp>

namespace ac {

template<typename Op, typename... PassedArgs>
    requires(!is_expression_v<Op>)
constexpr decltype(auto) pass_arguments(
    Op const& op, [[maybe_unused]] PassedArgs const&... args
) {
    if constexpr (Operation<Op>)
        return op(args...);
    else
        return op;
}

template<Operation Op, size_t... Is, typename... Args, typename... PassedArgs>
constexpr decltype(auto) pass_arguments(
    expression_data<Op, std::index_sequence<Is...>, Args...> const& expression,
    [[maybe_unused]] PassedArgs const&... args
) {
    if constexpr (ac::Operation<ac::expression<Op, Args...>>)
        return ac::expression{
            expression.operation,
            pass_arguments(std::get<Is>(expression.arguments), args...)...
        };
    else
        return expression;
}

template<typename Derived>
struct operation_base;

template<Operation Op, typename... Args>
    requires(... || ac::Operation<std::remove_cvref_t<Args>>)
class expression<Op, Args...>
    : public expression_data_t<Op, Args...>
    , public operation_base<expression<Op, Args...>> {
    using base_t = expression_data_t<Op, Args...>;

public:
    template<typename... Ts>
    using result_type = typename expression_result_type<decltype(pass_arguments(
        std::declval<expression>(), std::declval<Ts>()...
    ))>::type;

    using base_t::base_t;

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
