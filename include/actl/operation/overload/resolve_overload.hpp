// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/overload/overload.hpp>

namespace ac {

struct default_context
{};

template <class Void, class Context, class Op, class... Ts>
struct context_overload
{
    struct is_resolved;

    static constexpr auto resolve(Context, Op op)
    {
        return op;
    }

    template <class Op1>
    static constexpr auto& resolve(Context, Op1& op)
    {
        return op;
    }
};

template <class Void, class Context, class Op, class... Ts>
struct overload_resolver : context_overload<void, Context, Op, Ts...>
{};

template <class... Ts, class Context, class Op>
constexpr decltype(auto) resolve_overload(Context context, Op&& op)
{
    return overload_resolver<void, Context, raw_t<Op>, raw_t<Ts>...>::resolve(
        context, std::forward<Op>(op));
}

template <class... Ts, class Op>
constexpr decltype(auto) resolve_nested(const Op& op)
{
    constexpr auto major_depth = major_category<Ts...>::depth;
    return resolve_overload<
        detail::value_type_if_t<nesting_depth_v<Ts> == major_depth, Ts>...>(
        default_context{}, op);
}

namespace detail {

template <class Void, class... Ts>
struct is_overload_resolved : std::false_type
{};

template <class... Ts>
struct is_overload_resolved<
    std::void_t<typename overload_resolver<void, Ts...>::is_resolved>,
    Ts...> : std::true_type
{};

} // namespace detail

template <class... Ts>
inline constexpr bool is_overload_resolved_v =
    detail::is_overload_resolved<void, raw_t<Ts>...>::value;

template <class Context, class Op, class... Ts>
struct overload_resolver<
    std::void_t<decltype(
        primary_overload<Op, Ts...>::resolve(std::declval<Op>()))>,
    Context,
    Op,
    Ts...>
{
    template <class Op1>
    static constexpr auto resolve(Context context, Op1&& op)
    {
        return resolve_overload<Ts...>(
            context,
            primary_overload<Op, Ts...>::resolve(std::forward<Op1>(op)));
    }
};

} // namespace ac
