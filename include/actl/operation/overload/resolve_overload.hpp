// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/overload/overload.hpp>

namespace ac {

struct default_context {};

template <class Void, class... Ts>
struct is_overload_resolved : std::true_type {};

template <class... Ts>
inline constexpr bool is_overload_resolved_v =
    is_overload_resolved<void, raw_t<Ts>...>::value;

template <
    class... Ts,
    class Context,
    class Op,
    enable_int_if<!is_primary_overload_resolved_v<Op, Ts...>> = 0>
constexpr auto resolve_overload(Context context, Op&& op) {
    auto result = primary_overload<Op, Ts...>::resolve(std::forward<Op>(op));
    if constexpr (is_overload_resolved_v<Context, decltype(result), Ts...>)
        return std::move(result);
    else
        return resolve_overload<Ts...>(context, std::move(result));
}

template <class... Ts, class Context, class Op>
constexpr decltype(auto) resolve_overload_if_can(Context context, Op&& op) {
    if constexpr (is_overload_resolved_v<Context, Op, Ts...>)
        return std::forward<Op>(op);
    else
        return resolve_overload<Ts...>(context, std::forward<Op>(op));
}

template <class... Ts, class Op>
constexpr decltype(auto) resolve_nested(const Op& op) {
    constexpr auto major_depth = major_category<Ts...>::depth;
    return resolve_overload_if_can<
        detail::value_type_if_t<nesting_depth_v<Ts> == major_depth, Ts>...>(
        default_context{}, op);
}

template <class Context, class Op, class... Ts>
struct is_overload_resolved<
    std::void_t<decltype(
        resolve_overload<Ts...>(std::declval<Context>(), std::declval<Op>()))>,
    Context,
    Op,
    Ts...> : std::false_type {};

} // namespace ac
