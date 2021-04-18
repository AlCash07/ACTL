// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/scalar.hpp>
#include <actl/operation/core/argument_traits.hpp>
#include <actl/operation/core/operation_traits.hpp>
#include <actl/operation/out.hpp>
#include <actl/operation/overload/resolve.hpp>
#include <tuple>

namespace ac {

namespace detail {

template <bool, class Op, class... Ts>
struct expr_result1 {
    using type = decltype(std::declval<Op>().evaluate(std::declval<Ts>()...));
    using tag = category_t<remove_cvref_t<type>>;
};

template <class Op, class... Ts>
struct expr_result1<false, Op, Ts...> {
    using type = decltype(eval(resolve<Ts...>(
        default_context{}, std::declval<Op>())(std::declval<Ts>()...)));
    using tag = category_t<remove_cvref_t<type>>;
};

template <bool, class... Ts>
struct expr_result0
    : expr_result1<is_overload_resolved_v<default_context, Ts...>, Ts...> {};

template <class... Ts>
struct expr_result0<false, Ts...> {
    using tag = operation_tag;
};

template <class... Ts>
using expr_result =
    expr_result0<1 == (... + int{is_operation_v<remove_cvref_t<Ts>>}), Ts...>;

} // namespace detail

template <class Derived, class Tag>
struct expression_base : expression_base<Derived, typename Tag::base> {};

template <class Derived>
struct expression_base<Derived, unclassified_tag> {
    struct type {};
};

template <class Op, class... Ts>
struct expression
    : expression_base<
          expression<Op, Ts...>,
          typename detail::expr_result<Op, Ts...>::tag>::type {
    using category = typename detail::expr_result<Op, Ts...>::tag;
    struct enable_operators;

    static constexpr size_t argument_count = sizeof...(Ts);

    std::tuple<Op, Ts...> args;

    template <class... Us>
    constexpr expression(Us&&... xs) : args{std::forward<Us>(xs)...} {}

    constexpr auto& operation() const& {
        return std::get<0>(args);
    }

    constexpr auto&& operation() && {
        return std::get<0>(std::move(args));
    }
};

template <class... Ts>
expression(Ts&&...) -> expression<value_if_small<Ts>...>;

template <class... Ts>
struct expression_base<expression<Ts...>, scalar_tag> {
    struct type {
        constexpr operator typename detail::expr_result<Ts...>::type() const {
            return eval(static_cast<const expression<Ts...>&>(*this));
        }
    };
};

template <class T>
struct is_expression : std::false_type {};

template <class... Ts>
struct is_expression<expression<Ts...>> : std::true_type {};

template <class T>
inline constexpr bool is_expression_v = is_expression<remove_cvref_t<T>>::value;

template <class Expr>
using argument_indices =
    std::make_index_sequence<remove_cvref_t<Expr>::argument_count>;

template <class Expr, class S = argument_indices<Expr>>
struct expression_helper;

template <class Op, class... Ts, size_t... Is>
struct expression_helper<expression<Op, Ts...>, std::index_sequence<Is...>> {
    using Expr = expression<Op, Ts...>;

    static constexpr bool is_resolved =
        is_overload_resolved_v<default_context, Op, Ts...>;

    static constexpr auto resolve_expr(const Expr& e) {
        return expression{
            resolve<Ts...>(default_context{}, e.operation()),
            std::get<Is + 1>(e.args)...};
    }

    static constexpr decltype(auto) eval_impl(const Expr& e) {
        return e.operation().evaluate(std::get<Is + 1>(e.args)...);
    }

    template <class T>
    static constexpr void assign_impl(T& dst, const Expr& e) {
        e.operation().evaluate_to(dst, std::get<Is + 1>(e.args)...);
    }
};

template <class... Ts>
constexpr decltype(auto) eval(const expression<Ts...>& e) {
    using helper = expression_helper<expression<Ts...>>;
    if constexpr (helper::is_resolved)
        return helper::eval_impl(e);
    else
        return eval(helper::resolve_expr(e));
}

template <class T, class... Ts>
constexpr void assign(out<T>& dst, const expression<Ts...>& e) {
    using helper = expression_helper<expression<Ts...>>;
    if constexpr (helper::is_resolved)
        helper::assign_impl(dst.x, e);
    else
        assign(dst, helper::resolve_expr(e));
}

} // namespace ac
