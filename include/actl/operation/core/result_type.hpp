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
struct result_type
{
    using type = typename Op::template result_type<Ts...>;
};

template <class Op, class... Ts>
struct result_type<
    std::void_t<decltype(std::declval<Op>().evaluate(std::declval<Ts>()...))>,
    Op,
    Ts...>
{
    using type = decltype(std::declval<Op>().evaluate(std::declval<Ts>()...));
};

template <class T>
struct expression_result_type
{
    using type = T;
};

template <bool, class Op, class... Ts>
struct resolved_result_type1 : result_type<void, remove_cvref_t<Op>, Ts...>
{};

template <class Op, class... Ts>
struct resolved_result_type1<false, Op, Ts...>
    : expression_result_type<decltype(resolve_overload<Ts...>(
          default_context{}, std::declval<Op>())(std::declval<Ts>()...))>
{};

template <bool, class... Ts>
struct resolved_result_type0
    : resolved_result_type1<
          is_overload_resolved_v<default_context, Ts...>,
          Ts...>
{};

template <class... Ts>
struct resolved_result_type0<true, Ts...>
{
    using type = operation_tag;
};

template <class... Ts>
using resolved_result_type = resolved_result_type0<
    1 < (... + int{is_operation_v<remove_cvref_t<Ts>>}),
    Ts...>;

template <class... Ts>
using resolved_result_type_t = typename resolved_result_type<Ts...>::type;

template <class T>
inline constexpr bool is_category_v = is_subcategory_of_v<T, unclassified_tag>;

template <class T>
using category_or_self_t =
    std::conditional_t<is_category_v<T>, T, category_t<remove_cvref_t<T>>>;

template <class... Ts>
using resolved_result_category_t =
    category_or_self_t<resolved_result_type_t<Ts...>>;

} // namespace ac
