// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/core/argument_traits.hpp>
#include <actl/operation/core/operation_traits.hpp>
#include <actl/operation/core/out.hpp>
#include <tuple>

namespace ac {

namespace detail {

template <class Op, class... Ts>
struct result {
    using type =
        decltype(std::declval<Op>().template resolve<Ts...>().evaluate(std::declval<Ts>()...));
    using tag = category_t<remove_cvref_t<type>>;
};

}  // namespace detail

template <class Derived, class Tag>
struct expression_base {};

template <class Op, class... Ts>
struct expression
    : expression_base<expression<Op, Ts...>, typename detail::result<Op, Ts...>::tag> {
    using category = typename detail::result<Op, Ts...>::tag;
    struct enable_operators;

    std::tuple<Op, Ts...> args;

    constexpr decltype(auto) operation() const {
        return std::get<0>(args).template resolve<Ts...>();
    }

    constexpr operator typename detail::result<Op, Ts...>::type() const {
        return eval(*this);
    }
};

template <class... Ts, enable_int_if<1 == (... + int{is_operation_v<remove_cvref_t<Ts>>})> = 0>
constexpr auto make_expression(Ts&&... xs) {
    return expression<value_if_small<Ts>...>{{}, {std::forward<Ts>(xs)...}};
}

template <class E, size_t... Is>
constexpr decltype(auto) eval_impl(const E& e, std::index_sequence<Is...>) {
    return eval(e.operation().evaluate(std::get<Is + 1>(e.args)...));
}

template <class... Ts>
constexpr decltype(auto) eval(const expression<Ts...>& e) {
    return eval_impl(e, std::make_index_sequence<sizeof...(Ts) - 1>{});
}

template <class T, class E, size_t... Is>
constexpr void assign_impl(T& dst, const E& e, std::index_sequence<Is...>) {
    e.operation().evaluate_to(dst, std::get<Is + 1>(e.args)...);
}

template <class T, class... Ts>
constexpr void assign(out_t<T>& dst, const expression<Ts...>& e) {
    assign_impl(dst.x, e, std::make_index_sequence<sizeof...(Ts) - 1>{});
}

}  // namespace ac