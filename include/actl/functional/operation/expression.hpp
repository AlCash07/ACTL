/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/operation/operation_traits.hpp>
#include <actl/functional/operation/output_argument.hpp>
#include <actl/functional/operation/tuned_operation.hpp>
#include <tuple>

namespace ac::math {

namespace detail {

template <class Op, class... Ts>
struct result {
    using type = decltype(std::declval<Op>().evaluate(std::declval<Ts>()...));
    using tag = category_t<type>;
};

}  // namespace detail

template <class Derived, class Tag>
struct expression_base {};

template <class... Ts>
struct expression : expression_base<expression<Ts...>, typename detail::result<Ts...>::tag> {
    using category = typename detail::result<Ts...>::tag;

    std::tuple<Ts...> args;

    constexpr operator typename detail::result<Ts...>::type() const { return eval(*this); }
};

template <class... Ts>
struct expression_op : operation<expression_op<Ts...>> {
    std::tuple<Ts...> args;

    template <class... Us>
    explicit constexpr expression_op(Us&&... xs) : args{std::forward<Us>(xs)...} {}

    template <class... Us>
    constexpr auto evaluate(const Us&... xs) const {
        return eval(expand_expression(*this, xs...));
    }

    template <class T, class... Us>
    constexpr void evaluate_to(T& dst, const Us&... xs) const {
        assign(out(dst), expand_expression(*this, xs...));
    }
};

template <class T, class U = remove_cvref_t<T>>
using value_if_small = std::conditional_t<std::is_empty_v<U> || std::is_arithmetic_v<U>, U, T>;

template <class... Ts>
inline constexpr auto make_expression(Ts&&... xs) {
    if constexpr ((... + is_operation_v<Ts>) == 1) {
        return expression<value_if_small<Ts>...>{{}, {std::forward<Ts>(xs)...}};
    } else {
        return expression_op<value_if_small<Ts>...>{std::forward<Ts>(xs)...};
    }
}

template <class Op, class... Ts>
inline constexpr decltype(auto) expand_expression(const Op& op, const Ts&... xs) {
    if constexpr (!is_operation_v<Op>) {
        return op;
    } else {
        return op(xs...);
    }
}

template <class... Ts>
struct expression_helper : expression_helper<std::make_index_sequence<sizeof...(Ts) - 1>> {};

template <size_t... Is>
struct expression_helper<std::index_sequence<Is...>> {
    template <class EO, class... Us>
    static constexpr auto expand_impl(const EO& eop, const Us&... xs) {
        return make_expression(std::get<0>(eop.args),
                               expand_expression(std::get<Is + 1>(eop.args), xs...)...);
    }

    template <class E>
    static constexpr decltype(auto) eval_impl(const E& e) {
        return eval(std::get<0>(e.args).evaluate(std::get<Is + 1>(e.args)...));
    }

    template <class T, class E>
    static constexpr void assign_impl(T& dst, const E& e) {
        std::get<0>(e.args)(dst, std::get<Is + 1>(e.args)...);
    }

    template <class EO, class Policy>
    static constexpr auto apply_policy_impl(const EO& eop, const Policy policy) {
        return make_expression(apply_policy_if_can(std::get<0>(eop.args), policy),
                               apply_policy_if_can(std::get<Is + 1>(eop.args), policy)...);
    }
};

template <class... Ts, class... Us>
inline constexpr auto expand_expression(const expression_op<Ts...>& eop, const Us&... xs) {
    return expression_helper<Ts...>::expand_impl(eop, xs...);
}

template <class... Ts>
inline constexpr decltype(auto) eval(const expression<Ts...>& e) {
    return expression_helper<Ts...>::eval_impl(e);
}

template <class T, class... Ts>
inline constexpr void assign(out<false, T>& dst, const expression<Ts...>& e) {
    expression_helper<Ts...>::assign_impl(dst, e);
}

template <class... Ts, class Policy,
          enable_int_if<(... || can_apply_policy<Ts, Policy>::value)> = 0>
inline constexpr auto apply_policy(const expression_op<Ts...>& eop, const Policy& policy) {
    return expression_helper<Ts...>::apply_policy_impl(eop, policy);
}

}  // namespace ac::math
