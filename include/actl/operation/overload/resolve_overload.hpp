// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/none.hpp>
#include <actl/operation/overload/overload.hpp>
#include <algorithm>

namespace ac {

template<Operation Op, typename ArgsArray, typename... Policies>
struct policy_overload {
    struct is_resolved;

    static constexpr Op resolve(Op&& op, Policies const&...) {
        return std::move(op);
    }

    static constexpr Op const& resolve(Op const& op, Policies const&...) {
        return op;
    }
};

template<Operation Op, typename ArgsArray, typename... Policies>
struct overload_resolver : policy_overload<Op, ArgsArray, Policies...> {};

template<typename... Args, typename Op>
constexpr decltype(auto) resolve_overload(Op&& op) {
    return overload_resolver<raw_t<Op>, type_array<raw_t<Args>...>>::resolve(
        std::forward<Op>(op)
    );
}

template<Operation Op, typename ArgsArray, typename... Policies>
inline constexpr bool is_overload_resolved_v = requires {
    typename overload_resolver<raw_t<Op>, ArgsArray, Policies...>::is_resolved;
};

template<typename Op, typename... Args, typename... Policies>
    requires requires { overload<Op, Args...>::formula; }
struct overload_resolver<Op, type_array<Args...>, Policies...> {
    template<typename Op1>
    static constexpr auto resolve(Op1&& op, Policies const&... policies) {
        using Formula =
            std::remove_const_t<decltype(overload<Op, Args...>::formula)>;
        return overload_resolver<Formula, type_array<Args...>, Policies...>::
            resolve(Formula{}, policies...);
    }
};

} // namespace ac
