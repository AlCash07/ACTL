// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/nesting_depth.hpp>
#include <actl/operation/overload/overload.hpp>
#include <algorithm>

namespace ac {

struct default_context {};

template<typename Context, typename Op, typename... Ts>
struct context_overload {
    struct is_resolved;

    static constexpr auto resolve(Context, Op op) {
        return op;
    }

    template<typename Op1>
    static constexpr auto& resolve(Context, Op1& op) {
        return op;
    }
};

template<typename Context, typename Op, typename... Ts>
struct overload_resolver : context_overload<Context, Op, Ts...> {};

template<typename... Ts, typename Context, typename Op>
constexpr decltype(auto) resolve_overload(Context context, Op&& op) {
    return overload_resolver<Context, raw_t<Op>, raw_t<Ts>...>::resolve(
        context, std::forward<Op>(op)
    );
}

template<typename... Ts, Operation Op>
constexpr decltype(auto) resolve_nested(Op const& op) {
    constexpr auto max_depth = max_nesting_depth_v<Ts...>;
    return resolve_overload<
        detail::value_type_if_t<nesting_depth_v<Ts> == max_depth, Ts>...>(
        default_context{}, op
    );
}

template<typename... Ts>
inline constexpr bool is_overload_resolved_v =
    requires { typename overload_resolver<raw_t<Ts>...>::is_resolved; };

template<typename Context, Operation Op, typename... Ts>
    requires requires { overload<Op, Ts...>::formula; }
struct overload_resolver<Context, Op, Ts...> {
    template<typename Op1>
    static constexpr auto resolve(Context context, Op1&& op) {
        return resolve_overload<Ts...>(
            context,
            std::remove_const_t<decltype(overload<Op, Ts...>::formula)>{}
        );
    }
};

} // namespace ac
