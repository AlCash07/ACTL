// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/argument_traits.hpp>
#include <actl/operation/core/result_type.hpp>
#include <actl/operation/out.hpp>
#include <actl/operation/overload/resolve_overload.hpp>
#include <tuple>

namespace ac {

template <class Result, class = void>
struct can_convert_expression_implicitly : std::false_type
{};

template <class Derived, bool HasConversion = false>
struct expression_conversion
{};

template <class Derived, bool IsOperation = false>
struct operation_expression
{};

template <class Op, class... Ts>
struct expression
    : expression_conversion<
          expression<Op, Ts...>,
          can_convert_expression_implicitly<
              resolved_result_type_t<Op, Ts...>>::value>
    , operation_expression<
          expression<Op, Ts...>,
          std::is_same_v<operation_tag, resolved_result_type_t<Op, Ts...>>>
{
    using category = resolved_result_category_t<Op, Ts...>;
    struct enable_operators;

    static constexpr size_t argument_count = sizeof...(Ts);

    std::tuple<Op, Ts...> args;

    template <class... Us>
    constexpr expression(Us&&... xs) : args{std::forward<Us>(xs)...}
    {}

    constexpr auto& operation() const&
    {
        return std::get<0>(args);
    }

    constexpr auto&& operation() &&
    {
        return std::get<0>(std::move(args));
    }
};

template <class... Ts>
expression(Ts&&...) -> expression<value_if_small<Ts>...>;

template <class T>
struct is_expression : std::false_type
{};

template <class... Ts>
struct is_expression<expression<Ts...>> : std::true_type
{};

template <class T>
inline constexpr bool is_expression_v =
    is_expression<std::remove_cvref_t<T>>::value;

template <class Expr>
using argument_indices =
    std::make_index_sequence<std::remove_cvref_t<Expr>::argument_count>;

template <class... Ts>
struct expression_result_type<expression<Ts...>>
{
    using type = resolved_result_type_t<Ts...>;
};

template <class Expr>
struct expression_conversion<Expr, true>
{
    constexpr operator typename expression_result_type<Expr>::type() const
    {
        return eval(static_cast<Expr const&>(*this));
    }
};

template <class Expr, class S = argument_indices<Expr>>
struct expression_helper;

template <class Op, class... Ts, size_t... Is>
struct expression_helper<expression<Op, Ts...>, std::index_sequence<Is...>>
{
    using Expr = expression<Op, Ts...>;

    static constexpr bool is_resolved =
        is_overload_resolved_v<default_context, Op, Ts...>;

    static constexpr auto resolve_expr(Expr const& e)
    {
        return expression{
            resolve_overload<Ts...>(default_context{}, e.operation()),
            std::get<Is + 1>(e.args)...};
    }

    template <class T>
    static constexpr void assign_impl(T& dst, Expr const& e)
    {
        e.operation().evaluate_to(dst, std::get<Is + 1>(e.args)...);
    }
};

template <class T, class... Ts>
constexpr void assign(out<T>& dst, expression<Ts...> const& e)
{
    using helper = expression_helper<expression<Ts...>>;
    if constexpr (helper::is_resolved)
        helper::assign_impl(dst.x, e);
    else
        assign(dst, helper::resolve_expr(e));
}

} // namespace ac
