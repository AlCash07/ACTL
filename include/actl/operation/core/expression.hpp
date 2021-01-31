// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/argument_traits.hpp>
#include <actl/operation/core/operation_traits.hpp>
#include <actl/operation/core/out.hpp>
#include <actl/operation/core/overload.hpp>
#include <tuple>

namespace ac {

namespace detail {

template <bool, class Op, class... Ts>
struct result_type {
    using type = decltype(std::declval<Op>().evaluate(std::declval<Ts>()...));
    using tag = category_t<remove_cvref_t<type>>;
};

template <class Op, class... Ts>
struct result_type<false, Op, Ts...> {
    using type = decltype(
        eval(resolve<Ts...>(std::declval<Op>())(std::declval<Ts>()...)));
    using tag = category_t<remove_cvref_t<type>>;
};

template <class... Ts>
using result = result_type<is_overload_unchanged_v<Ts...>, Ts...>;

} // namespace detail

template <class Derived, class Tag>
struct expression_base {};

template <class Op, class... Ts>
struct expression
    : expression_base<
          expression<Op, Ts...>,
          typename detail::result<Op, Ts...>::tag> {
    using category = typename detail::result<Op, Ts...>::tag;
    struct enable_operators;

    static constexpr size_t argument_count = sizeof...(Ts);

    std::tuple<Op, Ts...> args;

    constexpr auto& operation() const {
        return std::get<0>(args);
    }

    constexpr operator typename detail::result<Op, Ts...>::type() const {
        return eval(*this);
    }
};

template <
    class... Ts,
    enable_int_if<1 == (... + int{is_operation_v<remove_cvref_t<Ts>>})> = 0>
constexpr auto make_expression(Ts&&... xs) {
    return expression<value_if_small<Ts>...>{{}, {std::forward<Ts>(xs)...}};
}

template <class E, class S = std::make_index_sequence<E::argument_count>>
struct expression_helper;

template <class Op, class... Ts, size_t... Is>
struct expression_helper<expression<Op, Ts...>, std::index_sequence<Is...>> {
    using E = expression<Op, Ts...>;

    static constexpr bool is_resolved = is_overload_unchanged_v<Op, Ts...>;

    static constexpr auto resolve(const E& e) {
        return ac::resolve<Ts...>(e.operation())(std::get<Is + 1>(e.args)...);
    }

    static constexpr decltype(auto) eval_impl(const E& e) {
        return eval(e.operation().evaluate(std::get<Is + 1>(e.args)...));
    }

    template <class T>
    static constexpr void assign_impl(T& dst, const E& e) {
        e.operation().evaluate_to(dst, std::get<Is + 1>(e.args)...);
    }
};

template <class... Ts>
constexpr decltype(auto) eval(const expression<Ts...>& e) {
    using helper = expression_helper<expression<Ts...>>;
    if constexpr (helper::is_resolved)
        return helper::eval_impl(e);
    else
        return eval(helper::resolve(e));
}

template <class T, class... Ts>
constexpr void assign(out_t<T>& dst, const expression<Ts...>& e) {
    using helper = expression_helper<expression<Ts...>>;
    if constexpr (helper::is_resolved)
        helper::assign_impl(dst.x, e);
    else
        assign(dst, helper::resolve(e));
}

} // namespace ac
