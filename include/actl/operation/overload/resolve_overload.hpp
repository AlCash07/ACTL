// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/overload/overload.hpp>
#include <algorithm>

namespace ac {

struct default_context {};

template<Operation Op, typename ArgsArray, typename Context>
struct context_overload {
    struct is_resolved;

    static constexpr Op resolve(Context, Op&& op) {
        return std::move(op);
    }

    static constexpr Op const& resolve(Context, Op const& op) {
        return op;
    }
};

template<Operation Op, typename ArgsArray, typename Context>
struct overload_resolver : context_overload<Op, ArgsArray, Context> {};

template<typename... Args, typename Context, typename Op>
constexpr decltype(auto) resolve_overload(Context context, Op&& op) {
    return overload_resolver<raw_t<Op>, type_array<raw_t<Args>...>, Context>::
        resolve(context, std::forward<Op>(op));
}

template<Operation Op, typename ArgsArray, typename Context>
inline constexpr bool is_overload_resolved_v = requires {
    typename overload_resolver<raw_t<Op>, ArgsArray, Context>::is_resolved;
};

template<typename Op, typename... Args, typename Context>
    requires requires { overload<Op, Args...>::formula; }
struct overload_resolver<Op, type_array<Args...>, Context> {
    template<typename Op1>
    static constexpr auto resolve(Context context, Op1&& op) {
        using Formula =
            std::remove_const_t<decltype(overload<Op, Args...>::formula)>;
        return overload_resolver<Formula, type_array<Args...>, Context>::
            resolve(context, Formula{});
    }
};

} // namespace ac
