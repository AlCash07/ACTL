// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/parameter/out.hpp>
#include <actl/memory/AC_NO_UNIQUE_ADDRESS.hpp>
#include <actl/operation/expression/argument_traits.hpp>
#include <actl/operation/expression/result_type.hpp>
#include <actl/operation/overload/resolve_overload.hpp>
#include <tuple>

namespace ac {

template<typename Result>
struct can_convert_expression_implicitly : std::false_type {};

template<typename Derived, bool IsOperation = false>
struct operation_expression {};

template<typename Operation, typename... Args>
struct expression
    : operation_expression<
          expression<Operation, Args...>,
          std::is_same_v<
              operation_tag,
              resolved_result_type_t<Operation, Args...>>> {
    static constexpr size_t argument_count = sizeof...(Args);

    AC_NO_UNIQUE_ADDRESS Operation operation;
    // TODO: avoid std::tuple because of slow compilation time
    // and move constructor never being trivial caused by a wording defect.
    AC_NO_UNIQUE_ADDRESS std::tuple<Args...> arguments;

    expression() = default;

    template<typename Op, typename... Ts>
        requires(std::is_constructible_v<Operation, Op &&> &&
                 sizeof...(Ts) == argument_count &&
                 (... && std::is_constructible_v<Args, Ts>))
    constexpr expression(Op&& op, Ts&&... args)
        : operation{std::forward<Op>(op)}
        , arguments{std::forward<Ts>(args)...} {}

    constexpr operator resolved_result_type_t<Operation, Args...>() const
        requires(can_convert_expression_implicitly<
                 resolved_result_type_t<Operation, Args...>>::value)
    {
        return eval(*this);
    }

    struct enable_operators;
};

template<typename... Ts>
expression(Ts&&...) -> expression<value_if_small<Ts>...>;

template<typename T>
struct is_expression : std::false_type {};

template<typename... Ts>
struct is_expression<expression<Ts...>> : std::true_type {};

template<typename T>
inline constexpr bool is_expression_v =
    is_expression<std::remove_cvref_t<T>>::value;

template<typename Expr>
using argument_indices =
    std::make_index_sequence<std::remove_cvref_t<Expr>::argument_count>;

template<typename... Ts>
struct expression_result_type<expression<Ts...>> {
    using type = resolved_result_type_t<Ts...>;
};

template<typename Expr, typename S = argument_indices<Expr>>
struct expression_helper;

template<typename Operation, typename... Args, size_t... Is>
struct expression_helper<
    expression<Operation, Args...>,
    std::index_sequence<Is...>> {
    using Expr = expression<Operation, Args...>;

    static constexpr bool is_resolved =
        is_overload_resolved_v<default_context, Operation, Args...>;

    static constexpr auto resolve_expr(Expr const& expr) {
        return expression{
            resolve_overload<Args...>(default_context{}, expr.operation),
            std::get<Is>(expr.arguments)...
        };
    }

    template<typename Target>
    static constexpr void assign_impl(Target& target, Expr const& expr) {
        expr.operation.evaluate_to(target, std::get<Is>(expr.arguments)...);
    }
};

template<typename T, typename... Ts>
constexpr void assign(out<T>& target, expression<Ts...> const& expr) {
    using helper = expression_helper<expression<Ts...>>;
    if constexpr (helper::is_resolved)
        helper::assign_impl(*target, expr);
    else
        assign(target, helper::resolve_expr(expr));
}

} // namespace ac
