// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/core/none.hpp>
#include <actl/operation/resolver/overload.hpp>
#include <algorithm>

namespace ac {

template<Operation Op, typename ArgsArray, typename... Policies>
struct policy_overload {
    struct is_resolved;

    constexpr Op operator()(Op&& op, Policies const&...) const {
        return std::move(op);
    }

    constexpr Op const& operator()(Op const& op, Policies const&...) const {
        return op;
    }
};

template<Operation Op, typename ArgsArray, typename... Policies>
struct operation_resolver : policy_overload<Op, ArgsArray, Policies...> {};

template<typename... Args, typename Op>
constexpr decltype(auto) resolve_operation(Op&& op) {
    return operation_resolver<raw_t<Op>, type_array<raw_t<Args>...>>{}(
        std::forward<Op>(op)
    );
}

template<Operation Op, typename ArgsArray, typename... Policies>
inline constexpr bool is_operation_resolved_v = requires {
    typename operation_resolver<raw_t<Op>, ArgsArray, Policies...>::is_resolved;
};

template<typename Op, typename... Args, typename... Policies>
    requires requires { overload<Op, Args...>::formula; }
struct operation_resolver<Op, type_array<Args...>, Policies...> {
    template<typename Op1>
    constexpr auto operator()(Op1&& op, Policies const&... policies) const {
        using Formula =
            std::remove_const_t<decltype(overload<Op, Args...>::formula)>;
        return operation_resolver<Formula, type_array<Args...>, Policies...>{}(
            Formula{}, policies...
        );
    }
};

} // namespace ac
