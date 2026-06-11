// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/constant.hpp>
#include <actl/operation/expression/evaluate.hpp>
#include <actl/operation/expression/raw.hpp>
#include <actl/operation/expression/value_if_cheap.hpp>

namespace ac {

/// Expression stores operation with its arguments for later evaluation.
///
/// If some of the arguments are also operations,
/// then expression becomes an operation itself
/// (implemented at the end of this file).
template<Operation Op, typename... Args>
class expression : public expression_data_t<Op, Args...> {
    using base_t = expression_data_t<Op, Args...>;
    using result_t = result_t<Op, Args...>;

public:
    using base_t::base_t;

    constexpr operator result_t() const
        // The result type should be cheap to create to do it implicitly.
        // The exact condition should be refined later.
        requires(is_constant_v<result_t> || std::is_scalar_v<result_t>)
    {
        return eval(*this);
    }

    struct enable_operators;
};

template<typename... Ts>
expression(Ts&&...) -> expression<value_if_cheap_t<Ts>...>;

template<typename... Ts>
struct is_expression<expression<Ts...>> : std::true_type {};

template<typename Derived>
struct operation_base;

template<Operation Op, typename... Args>
    requires(... || Operation<Args>)
class expression<Op, Args...>
    : public expression_data_t<Op, Args...>
    , public operation_base<expression<Op, Args...>> {
    using base_t = expression_data_t<Op, Args...>;

public:
    using base_t::base_t;
};

namespace detail {

template<typename T, typename... Args>
struct MakeExpression {
    static constexpr decltype(auto) call(T&& t, Args&&...) {
        return std::forward<T>(t);
    }
};
template<Operation Op, typename... Args>
struct MakeExpression<Op, Args...> {
    static constexpr auto call(Op&& op, Args&&... args) {
        return expression{std::forward<Op>(op), std::forward<Args>(args)...};
    }
};

template<bool IsLazy, typename T, typename... Args>
using PassArguments =
    std::conditional_t<IsLazy, MakeExpression<T, Args...>, Eval<T, Args...>>;

template<typename Expression, typename... Args>
struct EvalExpression;

template<Operation Op, size_t... Is, typename... StoredArgs, typename... Args>
struct EvalExpression<
    expression_data<Op, std::index_sequence<Is...>, StoredArgs...>,
    Args...> {
    static constexpr decltype(auto) call(
        expression<Op, StoredArgs...> const& expression, Args&&... args
    ) {
        auto&& resolved_op =
            resolve_operation<Op, result_t<StoredArgs const&, Args...>...>(
                expression.operation
            );
        using RawOp = std::remove_reference_t<decltype(resolved_op)>;
        return resolved_op.evaluate(
            PassArguments<
                RawOp::is_argument_maybe_unused(Is),
                StoredArgs const&,
                Args...>::call(std::get<Is>(expression.arguments), args...)...
        );
    }
};

} // namespace detail

template<Operation Op, typename... StoredArgs, typename... Args>
struct Eval<expression<Op, StoredArgs...>, Args...>
    : detail::EvalExpression<expression_data_t<Op, StoredArgs...>, Args...> {};

template<Operation Op, typename... StoredArgs, typename... Args>
struct Eval<expression<Op, StoredArgs...> const&, Args...>
    : detail::EvalExpression<expression_data_t<Op, StoredArgs...>, Args...> {};

template<typename Target, Operation Op, size_t... Is, typename... StoredArgs>
constexpr void assign(
    out<Target>& target,
    expression_data<Op, std::index_sequence<Is...>, StoredArgs...> const&
        expression
) {
    auto&& operation = resolve_operation<Op, result_t<StoredArgs const&>...>(
        expression.operation
    );
    using RawOp = std::remove_reference_t<decltype(operation)>;
    operation.evaluate_to(
        out{target},
        detail::PrepareArgument<
            RawOp::is_argument_maybe_unused(Is),
            StoredArgs const&>::call(std::get<Is>(expression.arguments))...
    );
}

} // namespace ac
