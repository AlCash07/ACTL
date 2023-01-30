// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/operation_traits.hpp>
#include <actl/operation/overload/resolve_overload.hpp>

namespace ac {

template <class Op, class... Ts>
struct result_type
{
    using type = typename Op::template result_type<Ts...>;
};

template <class Op, class... Ts>
    requires requires(Op op, Ts... xs) { op.evaluate(xs...); }
struct result_type<Op, Ts...>
{
    using type = decltype(std::declval<Op>().evaluate(std::declval<Ts>()...));
};

template <class T>
struct expression_result_type
{
    using type = T;
};

template <bool, class Op, class... Ts>
struct resolved_result_type1 : result_type<std::remove_cvref_t<Op>, Ts...>
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
inline constexpr bool has_operation_arg_v =
    1 < (... + int{Operation<std::remove_cvref_t<Ts>>});

template <class... Ts>
using resolved_result_type =
    resolved_result_type0<has_operation_arg_v<Ts...>, Ts...>;

template <class... Ts>
using resolved_result_type_t = typename resolved_result_type<Ts...>::type;

} // namespace ac
