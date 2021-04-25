// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/operation_traits.hpp>
#include <actl/operation/overload/resolve_overload.hpp>

namespace ac {

template <class Void, class Op, class... Ts>
struct result_type {
    using type = typename Op::template result_type<Ts...>;
};

template <class Op, class... Ts>
struct result_type<
    std::void_t<decltype(std::declval<Op>().evaluate(std::declval<Ts>()...))>,
    Op,
    Ts...> {
    using type = decltype(std::declval<Op>().evaluate(std::declval<Ts>()...));
};

template <class T>
struct expression_result_type {
    using type = T;
};

template <class Op, class... Ts>
constexpr decltype(auto) resolved_result_type() {
    if constexpr ((... || is_operation_v<remove_cvref_t<Ts>>))
        return operation_tag{};
    else if constexpr (is_overload_resolved_v<default_context, Op, Ts...>)
        return typename result_type<void, remove_cvref_t<Op>, Ts...>::type{};
    else {
        using E = decltype(resolve_overload<Ts...>(
            default_context{}, std::declval<Op>())(std::declval<Ts>()...));
        return typename expression_result_type<E>::type{};
    }
}

template <class... Ts>
using resolved_result_type_t = decltype(resolved_result_type<Ts...>());

template <class T>
inline constexpr bool is_category_v = is_subcategory_of_v<T, unclassified_tag>;

template <class T>
using category_or_self_t =
    std::conditional_t<is_category_v<T>, T, category_t<remove_cvref_t<T>>>;

template <class... Ts>
using resolved_result_category_t =
    category_or_self_t<resolved_result_type_t<Ts...>>;

} // namespace ac
